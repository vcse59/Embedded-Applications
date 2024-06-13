#ifndef __NETWORK_CLASS_INTERFACE_H__
#define __NETWORK_CLASS_INTERFACE_H__

#include "CommonClasses/CommonDefinitions.h"

namespace NetworkClass
{
    enum eLISTENING_MODE
    {
        POLL_MODE       = 0,
        SELECT_MODE     = 1,
        EPOLL_MODE      = 2,
        DEFAULT_MODE    = POLL_MODE
    };

    class NetworkClassInterface
    {
        public:
            NetworkClassInterface(){}
            virtual ~NetworkClassInterface(){}
            virtual COMMON_DEFINITIONS::eSTATUS createServer(enum NetworkClass::eLISTENING_MODE mode = NetworkClass::eLISTENING_MODE::DEFAULT_MODE) = 0;
            virtual void startClient() = 0;
            virtual COMMON_DEFINITIONS::eSTATUS sendMessage(int socket, const std::string& message) = 0;
            virtual COMMON_DEFINITIONS::eSTATUS receiveMessage(int socket, std::string& message) = 0;
            virtual COMMON_DEFINITIONS::eSTATUS closeSocket() = 0;
            virtual int getConnectionId() const = 0;

        protected:
            unsigned int mPortNumber;

        private:
            NetworkClassInterface(const NetworkClassInterface&) = delete;
            NetworkClassInterface& operator=(const NetworkClassInterface&) = delete;
            NetworkClassInterface(const NetworkClassInterface&&) = delete;
            NetworkClassInterface& operator=(const NetworkClassInterface&&) = delete;
    };
    typedef std::shared_ptr<NetworkClass::NetworkClassInterface> S_PTR_NETWORK_CLASS_INTERFACE;
}
#endif