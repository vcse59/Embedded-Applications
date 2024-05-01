#ifndef __NETWORK_CLASS_INTERFACE_H__
#define __NETWORK_CLASS_INTERFACE_H__

#include "CommonClasses/CommonDefinitions.h"

namespace NetworkClass
{
    class NetworkClassInterface
    {
        public:
            NetworkClassInterface(){}
            virtual ~NetworkClassInterface(){}
            virtual COMMON_DEFINITIONS::eSTATUS createServer() = 0;
            virtual COMMON_DEFINITIONS::eSTATUS connectToServer() = 0;
            virtual COMMON_DEFINITIONS::eSTATUS sendMessage(const char* message) = 0;
            virtual COMMON_DEFINITIONS::eSTATUS receiveMessage(const char* messageBuffer) = 0;
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