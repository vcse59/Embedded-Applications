#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <iomanip>
#include <openssl/sha.h>
#include <poll.h>
#include <sys/epoll.h>

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
    if (mServerSocket > 0)
    {
        LOGGER(m_logger) << "Closing the web server" << endl;
    }

	for (unsigned int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (mClientSockets[i] >= 0)
			close(mClientSockets[i]);
	}

	close(mServerSocket);
	mServerSocket = -1;

	memset(mClientSockets, -1, sizeof(int) * MAX_CONNECTIONS);
}

eSTATUS TCPServer::createServer(enum NetworkClass::eLISTENING_MODE mode)
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
        mStatus = COMMON_DEFINITIONS::eSTATUS::ERROR;
	}   

	//set master socket to allow multiple connections ,  
	//this is just a good habit, it will work without this  
	if( setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt,  
				sizeof(opt)) < 0 )   
	{   
		perror("setsockopt");   
        mStatus = COMMON_DEFINITIONS::eSTATUS::ERROR;
	}   

	//type of socket created  
	address.sin_family = AF_INET;   
	address.sin_addr.s_addr = INADDR_ANY;   
	address.sin_port = htons( mPortNumber );   

	//bind the socket to localhost port 8888  
	if (bind(mServerSocket, (struct sockaddr *)&address, sizeof(address))<0)   
	{   
		perror("bind failed");   
        mStatus = COMMON_DEFINITIONS::eSTATUS::ERROR;
	}   
	LOGGER(m_logger) << "Listener on port " << PORT << std::endl;   

	//try to specify maximum of 3 pending connections for the master socket  
	if (listen(mServerSocket, MAX_CONNECTIONS) < 0)   
	{   
		perror("listen");   
        mStatus = COMMON_DEFINITIONS::eSTATUS::ERROR;
	}   

    switch(mode)
    {
        case NetworkClass::eLISTENING_MODE::POLL_MODE:
            mStatus = usePoll();
            break;
        case NetworkClass::eLISTENING_MODE::SELECT_MODE:
            mStatus = useSelect();
            break;
        case NetworkClass::eLISTENING_MODE::EPOLL_MODE:
            mStatus = useEPoll();
            break;
        default :
            mStatus = usePoll();
    }

    return mStatus;
}

eSTATUS TCPServer::useSelect()
{
    LOGGER(m_logger) << "Use Select Call" << std::endl;
	int addrlen , new_socket , activity;   
	struct sockaddr_in address;

    isServerClosed = false;
	addrlen = sizeof(address);   
	LOGGER(m_logger) << "Waiting for connections ..." << std::endl;


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
        timeout.tv_sec = SERVER_LISTENDER_TIMEOUT_IN_MS;
        timeout.tv_usec = 0;

        // Wait for activity on the server socket
        int activity = select(mServerSocket + 1, &readfds, NULL, NULL, &timeout);
        if (activity == -1) {
            LOGGER(m_logger) << "Select error" << std::endl;
            mStatus = COMMON_DEFINITIONS::eSTATUS::ERROR;
            return mStatus;
        } else if (activity == 0) {
            continue;
        }

        // Accept incoming connection if available
        if (FD_ISSET(mServerSocket, &readfds)) {
            if ((client_socket = accept(mServerSocket, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
                LOGGER(m_logger) << "Accept failed" << std::endl;
                continue;
            }

	    char ipString[INET6_ADDRSTRLEN]; // Maximum length for IPv6 address string
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)(&client_addr);
            inet_ntop(AF_INET, &(ipv4->sin_addr), ipString, INET_ADDRSTRLEN);
            LOGGER(m_logger) << "New client connected : " << client_socket << "from " << ipString << std::endl;

        // Handle client request here
        // Spawn a new thread to handle the connection
        std::thread threadObject(&TCPServer::handle_connection, this, client_socket);
        threadObject.detach();
        }
    }
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

eSTATUS TCPServer::usePoll()
{
    LOGGER(m_logger) << "Use poll Call" << std::endl;
	int addrlen , new_socket , activity;   
	struct sockaddr_in address;

    isServerClosed = false;
	addrlen = sizeof(address);   
	LOGGER(m_logger) << "Waiting for connections ..." << std::endl;

    struct pollfd fds[MAX_CONNECTIONS + 1]; // Plus 1 for master socket
    memset(fds, 0, sizeof(fds));

	// Initialize pollfd structure for master socket
    fds[0].fd = mServerSocket;
   	fds[0].events = POLLIN;

	while(!isServerClosed)   
	{  
	    int new_socket;

	    // Call poll() to wait for events
        activity = poll(fds, MAX_CONNECTIONS + 1, SERVER_LISTENDER_TIMEOUT_IN_MS);
    
        if (activity == -1) {
            LOGGER(m_logger) << "poll error" << std::endl;
            mStatus = COMMON_DEFINITIONS::eSTATUS::ERROR;
            return mStatus;
        } else if (activity == 0) {
            continue;
        }
    
        // Check for incoming connection on master socket
        if (fds[0].revents & POLLIN) {
            if ((new_socket = accept(mServerSocket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                LOGGER(m_logger) << "accept failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            LOGGER(m_logger) << "New client connected : " << new_socket << "from IP Address : " << inet_ntoa(address.sin_addr) << " Port : " << ntohs(address.sin_port) << std::endl;

        // Handle client request here
        // Spawn a new thread to handle the connection
        std::thread threadObject(&TCPServer::handle_connection, this, new_socket);
        threadObject.detach();
        }
    }
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

eSTATUS TCPServer::useEPoll()
{
    LOGGER(m_logger) << "Use epoll Call" << std::endl;
	int addrlen , new_socket , activity, epoll_fd;
    struct  epoll_event events; 
	struct sockaddr_in address;

    isServerClosed = false;
	//accept the incoming connection  
	addrlen = sizeof(address);   
	LOGGER(m_logger) << "Waiting for connections ..." << std::endl;

    // Create epoll instance
    if ((epoll_fd = epoll_create1(0)) == -1)
    {
        LOGGER(m_logger) << "Fail to create instance of epoll" << std::endl;
        close(mServerSocket);
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    // Add server socket to epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = mServerSocket;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, mServerSocket, &ev) == -1)
    {
        LOGGER(m_logger) << "Fail to add server socket to epoll : " << errno << std::endl;
        close(mServerSocket);
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    while(!isServerClosed)
    {
        // Wait for events
        activity = epoll_wait(epoll_fd, &events, 1, SERVER_LISTENDER_TIMEOUT_IN_MS);
        if (activity == -1)
        {
            LOGGER(m_logger) << "epoll wait failed" << std::endl;
            close(mServerSocket);
            return COMMON_DEFINITIONS::eSTATUS::ERROR;
        }else if (activity == 0) {
            continue;
        }

        // Handle events
        if (events.data.fd == mServerSocket)
        {
            // Accept new connection
            if ( (new_socket = accept(mServerSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) == -1)
            {
                LOGGER(m_logger) << "Accept failed" << std::endl;
                close(mServerSocket);
                return COMMON_DEFINITIONS::eSTATUS::ERROR;
            }

            LOGGER(m_logger) << "New connection, socket fd is "<< new_socket << ", IP is : " << inet_ntoa(address.sin_addr) << " Port : " << ntohs(address.sin_port) << std::endl;

            // Handle client request here
            // Spawn a new thread to handle the connection
            std::thread threadObject(&TCPServer::handle_connection, this, new_socket);
            threadObject.detach();            
        }
    }
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

void TCPServer::handle_connection(int client_socket) {

    std::string receivedData;

    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();

    receiveMessage(client_socket, receivedData);

    HTTP_SERVICE::HttpParams params(m_logger, receivedData);
    std::string http_response = consoleApp->getHTTPSessionManager()->processHTTPMessage(params);

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
            LOGGER(m_logger) << "Error sending data\n";
            break;
        } else if (sent == 0) {
            // Connection closed by peer
            LOGGER(m_logger)  << "Connection closed by peer\n";
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
            LOGGER(m_logger) << "Receive timeout" << std::endl;
        } else {
            LOGGER(m_logger) << "Receiving error" << std::endl;
        }
    } else if (bytes_received == 0) {
        LOGGER(m_logger) << "Connection closed by peer" << std::endl;
    } else {
        // Process received data
        message += messageBuffer;
        memset(messageBuffer, 0, MAX_BUFFER_SIZE);
    }

    return eSTATUS::SUCCESS;
}

eSTATUS TCPServer::closeSocket()
{
    LOGGER(m_logger) << "Shutting down the web server" << endl;
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
