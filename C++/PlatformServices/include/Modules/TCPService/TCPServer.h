#ifndef __TCPNETWORK_H__
#define __TCPNETWORK_H__

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
#include "JsonModule/JsonItem.h"
#include "JsonModule/JSONParser.h"

namespace NetworkClass
{
    class TCPServer : public NetworkClass::NetworkClassInterface
    {
        public:

            TCPServer(LOGGER_SERVICE::S_PTR_LOGGER logger, unsigned int portNumber);
            ~TCPServer();

            COMMON_DEFINITIONS::eSTATUS createServer(enum NetworkClass::eLISTENING_MODE mode = NetworkClass::eLISTENING_MODE::DEFAULT_MODE) override;
            COMMON_DEFINITIONS::eSTATUS connectToServer() override;
            COMMON_DEFINITIONS::eSTATUS sendMessage(int socket, const std::string& message) override;
            COMMON_DEFINITIONS::eSTATUS receiveMessage(int socket, std::string& message) override;
            COMMON_DEFINITIONS::eSTATUS closeSocket() override;
            int getConnectionId() const override;

        protected:
            bool isServerClosed = {true};
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            int mServerSocket = {-1};
            int mClientSockets[MAX_CONNECTIONS] = {-1,};
            COMMON_DEFINITIONS::eSTATUS mStatus = COMMON_DEFINITIONS::eSTATUS::UNKNOWN;

            void  set_nonblock(int socket);
            // get sockaddr, IPv4 or IPv6:
            void* get_in_addr(struct sockaddr *sa);
            void handle_connection(int client_socket);

        private:
            TCPServer(const TCPServer&) = delete;
            TCPServer& operator=(const TCPServer&) = delete;
            TCPServer(const TCPServer&&) = delete;
            TCPServer& operator=(const TCPServer&&) = delete;

            COMMON_DEFINITIONS::eSTATUS useSelect();
            COMMON_DEFINITIONS::eSTATUS usePoll();
            COMMON_DEFINITIONS::eSTATUS useEPoll();
    };
}

#endif