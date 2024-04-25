#include "Modules/TCPService/TCPServer.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>

using namespace std;
using namespace NetworkClass;
using namespace COMMON_DEFINITIONS;

TCPServer::TCPServer(unsigned int portNumber)
{
    mPortNumber     =   portNumber;
}

TCPServer::~TCPServer()
{
    cout << "Close the TCP Socket" << endl;
    for (unsigned int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (mClientSockets[i] >= 0)
            close(mClientSockets[i]);
    }

    close(mServerSocket);
    mServerSocket = -1;

    memset(mClientSockets, -1, sizeof(int) * MAX_CONNECTIONS);
}

std::string TCPServer::parseHttpResponse(const char* httpResponse){

    char responseBody[MAX_BODY_SIZE] = {'\0',};
    // Find end of headers
    const char *headers_end = strstr(httpResponse, "\r\n\r\n");
    if (headers_end == NULL) {
        printf("Invalid HTTP response: no headers found\n");
        return std::string(responseBody);
    }

    // Print headers
    printf("HTTP Headers:\n");
    fwrite(httpResponse, 1, headers_end - httpResponse, stdout);
    printf("\n");

    // Print body, if any
    if (*(headers_end + 4) != '\0') {
        printf("HTTP Body:\n%s\n", headers_end + 4);
        strcpy(responseBody, headers_end + 4);
    } else {
        printf("No HTTP Body\n");
    }

    return std::string(responseBody);
}

eSTATUS TCPServer::createServer()
{
    int opt = 1;   
    int addrlen , new_socket , activity, i , valread , sd;   
    int max_sd;   
    struct sockaddr_in address;   
         
    char buffer[MAX_BUFFER_SIZE] = {'\0',};  //data buffer of 1K  
         
    //create a master socket  
    if( (mServerSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( mPortNumber );   
         
    //bind the socket to localhost port 8888  
    if (bind(mServerSocket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(mServerSocket, MAX_CONNECTIONS) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
    
    isServerClosed = false;
    //accept the incoming connection  
    addrlen = sizeof(address);   
    puts("Waiting for connections ...");   
         
    while(!isServerClosed)   
    {  
        int client_socket = -1;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        fd_set readfds;
        struct timeval timeout;

        // Clear the set and add server socket to it
        FD_ZERO(&readfds);
        FD_SET(mServerSocket, &readfds);

        // Set timeout
        timeout.tv_sec = 1;  // 5 seconds
        timeout.tv_usec = 0;

        // Wait for activity on the server socket
        int activity = select(mServerSocket + 1, &readfds, NULL, NULL, &timeout);
        if (activity == -1) {
            perror("Select error");
            mStatus = COMMON_DEFINITIONS::eSTATUS::ERROR;
            return mStatus;
        } else if (activity == 0) {
            continue;
        }

        // Accept incoming connection if available
        if (FD_ISSET(mServerSocket, &readfds)) {
            if ((client_socket = accept(mServerSocket, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
                perror("Accept failed");
                continue;
            }

            printf("New client connected : %d\n", client_socket);

            // Handle client request here
            // Spawn a new thread to handle the connection
            std::thread threadObject(&TCPServer::handle_connection, this, client_socket);
            threadObject.join();
        }
    }  

    return mStatus;
}

void TCPServer::handle_connection(int client_socket) {

    char buffer[MAX_BUFFER_SIZE] = {0,};
    std::string receivedData;
    // Receive data
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            std::cerr << "Receive timeout" << std::endl;
        } else {
            std::cerr << "Receiving error" << std::endl;
        }
    } else if (bytes_received == 0) {
        std::cerr << "Connection closed by peer" << std::endl;
    } else {
        // Process received data
        receivedData += buffer;
        memset(buffer, 0, MAX_BUFFER_SIZE);
    }

    std::cout << "Received HTTP request:\n" << receivedData << std::endl;

    std::string responseBody = parseHttpResponse(receivedData.c_str());

    if (responseBody.length() > 0){
        std::shared_ptr<std::string> jsonString = std::make_shared<std::string>(responseBody);
        std::shared_ptr<PARSER_INTERFACE::DataParserInterface> parser = std::make_shared<JSON_SERVICE::jsonParser>(jsonString);
        parser->deserialize();
        std::cout << parser->dumpData() << std::endl;
    }

    // Read index.html content
    std::string index_html_content = readIndexHtml("webFiles/index.html");

    // Generate HTTP response
    std::string http_response = generateHttpResponse(index_html_content);
    http_response[http_response.length()] = '\0';

    std::cout << "Sending response : " << http_response << std::endl;

    // Send HTTP response
    const char* data = http_response.c_str();
    size_t remaining = http_response.size();
    while (remaining > 0) {
        ssize_t sent = send(client_socket, data, remaining, 0);
        if (sent == -1) {
            // Handle send error
            std::cerr << "Error sending data\n";
            break;
        } else if (sent == 0) {
            // Connection closed by peer
            std::cerr << "Connection closed by peer\n";
            break;
        } else {
            // Advance buffer pointer and update remaining data size
            data += sent;
            remaining -= sent;
        }
    }

    close(client_socket);
}

COMMON_DEFINITIONS::eSTATUS TCPServer::connectToServer()
{
    return eSTATUS::SUCCESS;
}

eSTATUS TCPServer::sendMessage(const char* message)
{
    return eSTATUS::SUCCESS;
}

eSTATUS TCPServer::receiveMessage(const char* messageBuffer)
{
    return eSTATUS::SUCCESS;
}

eSTATUS TCPServer::closeSocket()
{
    cout << "Shutting down the server" << endl;
    isServerClosed = true;
    return eSTATUS::SUCCESS;
}

int TCPServer::getConnectionId() const
{
    return -1;
}

void  TCPServer::set_nonblock(int socket)
{
    int flags;
    flags = fcntl(socket,F_GETFL,0);
    assert(flags != -1);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void* TCPServer::get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

JSON_SERVICE::JsonItem TCPServer::parseResponse(std::string httpBody)
{
    return JSON_SERVICE::JsonItem();
}

std::string TCPServer::readIndexHtml(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Output the contents of the file
    std::cout << "Contents of index.html:\n" << buffer.str() << std::endl;

    return buffer.str();
}

std::string TCPServer::generateHttpResponse(const std::string& content) {
    std::cout << "CONTENT LENGTH***************" << content.length() << std::endl;
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << content.size() << "\r\n\r\n";
    response << content;
    return response.str();
}