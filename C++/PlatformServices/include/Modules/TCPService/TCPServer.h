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

            TCPServer(unsigned int portNumber);
            ~TCPServer();

            COMMON_DEFINITIONS::eSTATUS createServer() override;
            COMMON_DEFINITIONS::eSTATUS connectToServer() override;
            COMMON_DEFINITIONS::eSTATUS sendMessage(const char* message) override;
            COMMON_DEFINITIONS::eSTATUS receiveMessage(const char* messageBuffer) override;
            COMMON_DEFINITIONS::eSTATUS closeSocket() override;
            int getConnectionId() const override;

        protected:
            bool isServerClosed = {true}; 
            int mServerSocket = {-1};
            int mClientSockets[MAX_CONNECTIONS] = {-1,};
            COMMON_DEFINITIONS::eSTATUS mStatus = COMMON_DEFINITIONS::eSTATUS::UNKNOWN;

            void  set_nonblock(int socket);
            // get sockaddr, IPv4 or IPv6:
            void* get_in_addr(struct sockaddr *sa);
            void handle_connection(int client_socket);
            JSON_SERVICE::JsonItem parseResponse(std::string httpBody);
            std::string parseHttpResponse(const char* httpResponse);
    };
}

#endif