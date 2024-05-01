#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <iomanip>
#include <openssl/sha.h>

#include "Modules/TCPService/TCPServer.h"
#include "Modules/ConsoleMain.h"
#include "Modules/HTTPService/HTTPParams.h"

using namespace std;
using namespace NetworkClass;
using namespace COMMON_DEFINITIONS;

TCPServer::TCPServer(LOGGER_SERVICE::S_PTR_LOGGER logger, unsigned int portNumber)
{
    m_logger        =   logger;
    mPortNumber     =   portNumber;
}

TCPServer::~TCPServer()
{
    cout << "Close the web server" << endl;
    for (unsigned int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (mClientSockets[i] >= 0)
            close(mClientSockets[i]);
    }

    close(mServerSocket);
    mServerSocket = -1;

    memset(mClientSockets, -1, sizeof(int) * MAX_CONNECTIONS);
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

	    char ipString[INET6_ADDRSTRLEN]; // Maximum length for IPv6 address string
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)(&client_addr);
            inet_ntop(AF_INET, &(ipv4->sin_addr), ipString, INET_ADDRSTRLEN);
            printf("New client connected : %d from %s\n", client_socket, ipString);

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

    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();

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

    HTTP_SERVICE::HttpParams params(m_logger);
    params.parseHttpResponse(receivedData);

    std::cout << "Method : " << params.getHTTPMethods() << std::endl;
    std::cout << "Host IP : " << params.getHostIP() << std::endl;

    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    std::string responseBody = httpUtility->parseHttpResponse(receivedData.c_str());
    std::string sessionId;
    std::unordered_map<std::string, std::string> headerMap = httpUtility->parseHttpHeaders(receivedData, sessionId);

    std::unordered_map<std::string, std::string>::iterator it = headerMap.begin();
    //std::cout << "HEADER START" << std::endl;
    //std::cout << "Session ID : " << sessionId << std::endl;
    /*while(it != headerMap.end()){
        std::cout << it->first << " " << it->second << std::endl;
        it++;
    }*/
    //std::cout << "HEADER END" << std::endl;

    COMMON_DEFINITIONS::eHTTP_SESSION_STATUS httpStatus = consoleApp->getHTTPSessionManager()->isValidSession(sessionId);
    COMMON_DEFINITIONS::eHTTP_SESSION_STATUS sessionStatus = consoleApp->getHTTPSessionManager()->addSession(sessionId);
    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "SESSION STATUS : " << httpStatus << std::endl;
    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "SESSION ID : " << sessionId << std::endl;

    if (responseBody.length() > 0){
        std::shared_ptr<std::string> jsonString = std::make_shared<std::string>(responseBody);
        std::shared_ptr<PARSER_INTERFACE::DataParserInterface> parser = std::make_shared<JSON_SERVICE::jsonParser>(m_logger, jsonString);
        parser->deserialize();
        //std::cout << parser->dumpData() << std::endl;
    }

    // Read index.html content
    std::string index_html_content = httpUtility->readIndexHtml("webFiles/index.html");

    // Generate HTTP response
    std::string http_response = httpUtility->generateHttpResponse(index_html_content, sessionId);
    //std::cout << "Sending response : " << http_response << std::endl;

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
    cout << "Shutting down the web server" << endl;
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
