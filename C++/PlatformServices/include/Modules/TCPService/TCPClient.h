#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <cstdint>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/time.h>
#include <unistd.h> // read(), write(), close()
#include <fcntl.h>
#include <assert.h>
#include <iostream>
#include <errno.h>  
#include <arpa/inet.h>    //close  
#include <string.h>

#include "Interfaces/NetworkClassInterface.h"

namespace NetworkClass
{
    class TCPClient : public NetworkClass::NetworkClassInterface
    {
        public:
            TCPClient(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string serverAddress, unsigned int portNumber);
            ~TCPClient();

            COMMON_DEFINITIONS::eSTATUS createServer() override;
            COMMON_DEFINITIONS::eSTATUS connectToServer() override;
            COMMON_DEFINITIONS::eSTATUS sendMessage(int socket, const std::string& message) override;
            COMMON_DEFINITIONS::eSTATUS receiveMessage(int socket, std::string& message) override;
            COMMON_DEFINITIONS::eSTATUS closeSocket() override;
            int getConnectionId() const override;
        
        private:
            int m_Socketfd = -1;
            std::string m_SessionId;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            struct sockaddr_in m_ServerAddress;
            std::string m_ServerIPAddress;
            bool m_CloseConnection = {false}; 
            COMMON_DEFINITIONS::eSTATUS m_SocketStatus = COMMON_DEFINITIONS::eSTATUS::SOCKET_INITIALIZATION_FAILED;
            void exchangeMessages();
            TCPClient(const TCPClient&) = delete;
            TCPClient& operator=(const TCPClient&) = delete;
            TCPClient(const TCPClient&&) = delete;
            TCPClient& operator=(const TCPClient&&) = delete;
    };
}

#endif