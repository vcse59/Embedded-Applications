/**
 * @file ConsoleAppInterface.h
 *
 * @brief This file declares the interface for Console application
 *
 * @author Vivek Yadav
 * Contact: v.cse59@gmail.com
 *
 */

#ifndef __CONSOLE_APP_INTERFACE_H__
#define __CONSOLE_APP_INTERFACE_H__

// Include header files
#include "Interfaces/NetworkClassInterface.h"
#include "Modules/TCPService/TCPServer.h"
#include "Modules/TCPService/TCPClient.h"
#include "Modules/HTTPService/HTTPUtility.h"
#include "Containers/Queue.h"
#include "Modules/HTTPService/HTTPSessionManager.h"
#include "Modules/Logger/Logger.h"

namespace FRAMEWORK
{
    class ConsoleAppInterface
    {
        public:

            ConsoleAppInterface(){}

            // Destructor
            virtual ~ConsoleAppInterface(){}

            // Returns NetworkClassInterface class singleton instance
            virtual NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPServer() = 0;

            // Returns NetworkClassInterface class singleton instance
            virtual NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPClient() = 0;

            // Returns QueueContainer class singleton instance
            virtual Storage::QueueContainer<COMMON_DEFINITIONS::SingleLLNode> &getQueue() = 0;
            
            // Returns SingleLinkedList class singleton instance
            virtual Storage::SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode> &getSingleLinkedList() = 0;

            // Returns HttpParser class singleton instance
            virtual HTTP_SERVICE::S_PTR_HTTP_UTILITY &getHTTPUtility() = 0;

            // Returns HttpSessionManager class singleton instance
            virtual HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER &getHTTPSessionManager() = 0;

            // Returns Logger class singleton instance
            virtual LOGGER_SERVICE::S_PTR_LOGGER &getLogger() = 0;

        protected:

            NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE m_nwInterface = nullptr;
            Storage::QueueContainer<COMMON_DEFINITIONS::SingleLLNode>* m_queueInterface = nullptr;
            Storage::SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode> *m_linkedList = nullptr;
            HTTP_SERVICE::S_PTR_HTTP_UTILITY m_HttpUtility = nullptr;
            HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER m_HttpSesssionManager = nullptr;
            LOGGER_SERVICE::S_PTR_LOGGER m_Logger = nullptr;

        private:
            ConsoleAppInterface(const ConsoleAppInterface&) = delete;
            ConsoleAppInterface& operator=(const ConsoleAppInterface&) = delete;
            ConsoleAppInterface(const ConsoleAppInterface&&) = delete;
            ConsoleAppInterface& operator=(const ConsoleAppInterface&&) = delete;
    };

    typedef std::shared_ptr<FRAMEWORK::ConsoleAppInterface> S_PTR_CONSOLEAPPINTERFACE;
}

#endif