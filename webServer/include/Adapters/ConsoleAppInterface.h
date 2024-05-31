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
#include "Interfaces/DataBaseTableInterface.h"
#include "Interfaces/DataBaseConnectorInterface.h"
#include "Interfaces/NetworkClassInterface.h"
#include "Modules/TCPService/TCPServer.h"
#include "Modules/TCPService/TCPClient.h"
#include "Modules/HTTPService/HTTPUtility.h"
#include "Modules/HTTPService/HTTPSessionManager.h"
#include "Modules/Logger/Logger.h"
#include "Modules/EventMessage/DBEventQueue.h"

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

            // Returns DataBaseTableInterface class singleton instance
            virtual DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE getDataBaseTable() = 0;

            // Returns DataBaseConnectorInterface class singleton instance
            virtual DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE getDBInstance() = 0;

            // Returns HttpParser class singleton instance
            virtual HTTP_SERVICE::S_PTR_HTTP_UTILITY &getHTTPUtility() = 0;

            // Returns HttpSessionManager class singleton instance
            virtual HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER &getHTTPSessionManager() = 0;

            // Returns Logger class singleton instance
            virtual LOGGER_SERVICE::S_PTR_LOGGER &getLogger() = 0;

            // Returns DB Queue class singleton interface
            virtual EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getDBQueueInterface() = 0;

        protected:

            NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE m_nwInterface = nullptr;
            DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE m_dbTableInterface = nullptr;
            DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE m_dbConnector = nullptr;
            HTTP_SERVICE::S_PTR_HTTP_UTILITY m_HttpUtility = nullptr;
            HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER m_HttpSesssionManager = nullptr;
            LOGGER_SERVICE::S_PTR_LOGGER m_Logger = nullptr;
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE m_dbQueueInterface = nullptr;

        private:
            ConsoleAppInterface(const ConsoleAppInterface&) = delete;
            ConsoleAppInterface& operator=(const ConsoleAppInterface&) = delete;
            ConsoleAppInterface(const ConsoleAppInterface&&) = delete;
            ConsoleAppInterface& operator=(const ConsoleAppInterface&&) = delete;
    };

    typedef std::shared_ptr<FRAMEWORK::ConsoleAppInterface> S_PTR_CONSOLEAPPINTERFACE;
}

#endif