#include <sstream>
#include <thread>
#include <chrono>

#include "Modules/TCPService/TCPClient.h"

using namespace NetworkClass;
using namespace std;

TCPClient::TCPClient( std::string serverAddress, unsigned int portNumber)
{
    m_ServerIPAddress   =   serverAddress;
    mPortNumber         =   portNumber;
}

TCPClient::~TCPClient()
{
    closeSocket();
}

COMMON_DEFINITIONS::eSTATUS TCPClient::createServer(enum NetworkClass::eLISTENING_MODE mode)
{
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

void TCPClient::startClient()
{
    #define MAXTHREAD   1000
    std::shared_ptr<std::thread> th[MAXTHREAD];
    for (int i = 0; i < MAXTHREAD; i++)
    {
        th[i] = std::make_shared<std::thread>(&TCPClient::run, this);
    }
    for (int i = 0; i < MAXTHREAD; i++)
    {
        th[i]->join();
    }
}

void TCPClient::run()
{
        int socket_fd = -1;
        struct sockaddr_in ServerAddress;
        m_SessionId = "5344de763fe60e4a4477d0a043efa3ba";

        // Create a socket
        if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            std::cout << "Socket creation failed" << std::endl;
            return;
        }

        // Initialize server address structure
        ServerAddress.sin_family = AF_INET;
        ServerAddress.sin_port = htons(mPortNumber);
        ServerAddress.sin_addr.s_addr = inet_addr(m_ServerIPAddress.c_str());

        // Connect to server
        if (connect(socket_fd, (struct sockaddr *)&ServerAddress, sizeof (ServerAddress)) == -1)
        {
            std::cout << "Connection failed" << std::endl;
            closeSocket(socket_fd);
            return;
        }

        std::cout << "Connection to server : " << m_ServerIPAddress << " at PORT : " << mPortNumber << " is CONNECTED" << std::endl;        
        exchangeMessages(socket_fd);
}

COMMON_DEFINITIONS::eSTATUS TCPClient::sendMessage(int socket, const std::string& message)
{
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS TCPClient::receiveMessage(int socket, std::string& message)
{
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS TCPClient::closeSocket(int socket_fd)
{
    if (socket_fd != -1)
        close(socket_fd);
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS TCPClient::closeSocket()
{
    if (m_Socketfd != -1)
        close(m_Socketfd);
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

int TCPClient::getConnectionId() const
{
    return 1;
}

void TCPClient::exchangeMessages(int socket_fd)
{
    // Send HTTP request
    // JSON data to send
    const char *json_data = "{\"name\": \"John\", \"age\": 300}";

    // Construct HTTP request with JSON data
    char http_request[MAX_BUFFER_SIZE];
    snprintf(http_request, MAX_BUFFER_SIZE,
             "GET /login HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/json\r\n"
             "Set-Cookie: sessionID=%s; HttpOnly; Secure\r\n" // Embed session ID in cookie
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             "127.0.0.1", m_SessionId.c_str(), strlen(json_data), json_data);

    http_request[strlen(http_request)] = '\0';
    // Send HTTP response
    const char* data = http_request;
    size_t remaining = strlen(http_request);
    while (remaining > 0) {
        ssize_t sent = send(socket_fd, data, remaining, 0);
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

    //sleep(2);

    // Receive HTTP response
    char buffer[MAX_BUFFER_SIZE] = {0,};
    std::string receivedData;
    // Receive data
    ssize_t bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);
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

    if (bytes_received == -1) {
        std::cerr << "Error receiving response" << std::endl;
        return;
    }

    std::string response = receivedData;
    std::cout << std::endl << "Response:\n" << response << std::endl;
    close(socket_fd);
}