#include <sstream>

#include "Modules/TCPService/TCPClient.h"
#include "JsonModule/JSONParser.h"
#include "JsonModule/JsonItem.h"

using namespace NetworkClass;

TCPClient::TCPClient(std::string serverAddress, unsigned int portNumber)
{
    m_ServerIPAddress   =   serverAddress;
    mPortNumber         =   portNumber;

    // Create a socket
    if ((m_Socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout <<"Socket creation failed" << std::endl;
        return;
    }

    // Initialize server address structure
    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_port = htons(portNumber);
    m_ServerAddress.sin_addr.s_addr = inet_addr(serverAddress.c_str());

    m_SocketStatus = COMMON_DEFINITIONS::eSTATUS::SOCKET_CREATED;
}

TCPClient::~TCPClient()
{
    closeSocket();
}

COMMON_DEFINITIONS::eSTATUS TCPClient::createServer()
{
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS TCPClient::connectToServer()
{
    if (m_SocketStatus == COMMON_DEFINITIONS::eSTATUS::SOCKET_CREATED){
        // Connect to server
        if (connect(m_Socketfd, (struct sockaddr *)&m_ServerAddress, sizeof (m_ServerAddress)) == -1)
        {
            perror("Connect failed");
            std::cout << "Connection failed" << std::endl;
            closeSocket();
            return COMMON_DEFINITIONS::eSTATUS::ERROR;
        }

        std::cout << "Connection to server : " << m_ServerIPAddress << " at PORT : " << mPortNumber << " is CONNECTED" << std::endl;        
        exchangeMessages();
    }else{
        m_SocketStatus = COMMON_DEFINITIONS::eSTATUS::SOCKET_INITIALIZATION_FAILED;
    }

    return m_SocketStatus;
}

COMMON_DEFINITIONS::eSTATUS TCPClient::sendMessage(const char* message)
{
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS TCPClient::receiveMessage(const char* messageBuffer)
{
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

void TCPClient::exchangeMessages()
{
    // Send HTTP request
    // JSON data to send
    const char *json_data = "{\"name\": \"John\", \"age\": 300}";

    // Construct HTTP request with JSON data
    char http_request[MAX_BUFFER_SIZE];
    snprintf(http_request, MAX_BUFFER_SIZE,
             "POST /api HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             "127.0.0.1", strlen(json_data), json_data);
    ssize_t sent = send(m_Socketfd, http_request, sizeof(http_request), 0);
    if (sent == -1) {
        std::cerr << "Error sending request" << std::endl;
        return;
    }

    // Receive HTTP response
    char buffer[MAX_BUFFER_SIZE];
    std::stringstream response_stream;
    ssize_t bytes_received;
    while ((bytes_received = recv(m_Socketfd, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
        response_stream.write(buffer, bytes_received);
    }

    if (bytes_received == -1) {
        std::cerr << "Error receiving response" << std::endl;
        return;
    }

    std::string response = response_stream.str();
    std::cout << std::endl << "Response:\n" << response << std::endl;
    close(m_Socketfd);
}