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

    std::string receivedData;

    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();

    receiveMessage(client_socket, receivedData);

    std::cout << "Received HTTP request:\n" << receivedData << std::endl;

    HTTP_SERVICE::HttpParams params(m_logger, receivedData);
    consoleApp->getHTTPSessionManager()->processHTTPMessage(params);

    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();
    // Read index.html content
    std::string index_html_content = httpUtility->readIndexHtml("webFiles/index.html");

    // Generate HTTP response
    std::string sessionId;
    std::string http_response = httpUtility->generateHttpResponse(index_html_content, sessionId);
    //std::cout << "Sending response : " << http_response << std::endl;

    sendMessage(client_socket, http_response.c_str());

    close(client_socket);
}

COMMON_DEFINITIONS::eSTATUS TCPServer::connectToServer()
{
    return eSTATUS::SUCCESS;
}

eSTATUS TCPServer::sendMessage(int socket, const std::string& message)
{
    // Send HTTP response
    const char* data = message.c_str();
    size_t remaining = message.length();
    while (remaining > 0) {
        ssize_t sent = send(socket, data, remaining, 0);
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

    return eSTATUS::SUCCESS;
}

eSTATUS TCPServer::receiveMessage(int socket, std::string& message)
{
    char messageBuffer[MAX_BUFFER_SIZE] = {0,};

    // Receive data
    ssize_t bytes_received = recv(socket, messageBuffer, sizeof(messageBuffer), 0);
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
        message += messageBuffer;
        memset(messageBuffer, 0, MAX_BUFFER_SIZE);
    }

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
