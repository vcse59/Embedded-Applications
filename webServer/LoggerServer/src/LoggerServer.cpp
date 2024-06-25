/**
 * @file LoggerServer.cpp
 *
 * @brief This file defines LoggerServer class to demonstrate
 *        Collecting logs
 *
 * @author Vivek Yadav
 * Contact: v.cse59@gmail.com
 *
 */
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "LoggerServer.h"

#define BUFFER_SIZE 1024


using namespace LOGGER_SERVICE;

LoggerServer::LoggerServer()
{

}

LoggerServer::~LoggerServer()
{

}

COMMON_DEFINITIONS::eSTATUS LoggerServer::startServer()
{
    int  clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create socket
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerSocket < 0)
    {
        perror("Error opening socket");
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(COMMON_DEFINITIONS::LOG_SERVER_PORT);

    // Bind the socket
    if (bind(mServerSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Error binding socket");
        close(mServerSocket);
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    // Listen for incoming connections
    if (listen(mServerSocket, 5) < 0)
    {
        perror("Error listening on socket");
        close(mServerSocket);
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    std::cout << "Server listening on port " << COMMON_DEFINITIONS::LOG_SERVER_PORT << std::endl;

    // Main server loop
    while (true)
    {
        // Accept an incoming connection
        clientSocket = accept(mServerSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket < 0)
        {
            perror("Error accepting connection");
            continue;
        }

        std::cout << "Accepted a connection" << std::endl;

        // Handle the client in a separate function
        handleLoggerClient(clientSocket);
    }

    // Close the server socket
    close(mServerSocket);
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

void LoggerServer::handleLoggerClient(int clientSocket)
{
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while (true)
    {
        // Read message from the client
        bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
        if (bytesRead < 0)
        {
            perror("Error reading from socket");
            break;
        }
        else if (bytesRead == 0)
        {
            // Client has closed the connection
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        buffer[bytesRead] = '\0'; // Null-terminate the buffer
        std::cout << "Received: " << buffer << std::endl;
    }

    // Close the client socket when done
    close(clientSocket);
}