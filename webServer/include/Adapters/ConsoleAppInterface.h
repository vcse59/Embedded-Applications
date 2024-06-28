/**
 * @file ConsoleAppInterface.h
 * @brief Header file for the ConsoleAppInterface class
 *
 * This file contains the declaration of the ConsoleAppInterface class,
 * which demonstrates basic C++ Abstract class implementation
 *
 * Contact: Vivek Yadav <v.cse59@gmail.com>
 *
 * @author Vivek Yadav
 * @date 2024-06-12
 */

#ifndef __CONSOLE_APP_INTERFACE_H__
#define __CONSOLE_APP_INTERFACE_H__

// Include header files
#ifdef _ENABLE_DB_
#include "Interfaces/DataBaseTableInterface.h"
#include "Interfaces/DataBaseConnectorInterface.h"
#endif
#include "Interfaces/NetworkClassInterface.h"
#include "Interfaces/EventQueueInterface.h"
#include "Modules/HTTPService/HTTPUtility.h"
#include "Modules/HTTPService/HTTPSessionManager.h"

namespace FRAMEWORK
{
    class ConsoleAppInterface
    {
        public:

            ConsoleAppInterface(){}

            // Destructor
            virtual ~ConsoleAppInterface(){
            }

            // Returns NetworkClassInterface class singleton instance
            virtual NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPServer() = 0;

#ifdef _ENABLE_DB_
            // Returns DataBaseConnectorInterface class singleton instance
            virtual DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE getDBInstance() = 0;

            // Returns DB Queue class singleton interface
            virtual EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getDBQueueInterface() = 0;
#endif
            // Returns HttpParser class singleton instance
            virtual HTTP_SERVICE::S_PTR_HTTP_UTILITY &getHTTPUtility() = 0;

            // Returns HttpSessionManager class singleton instance
            virtual HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER &getHTTPSessionManager() = 0;

            // Returns Logger class singleton instance
            virtual LOGGER_SERVICE::S_PTR_LOGGER &getLogger() = 0;

            // Returns Logger Queue class singleton interface
            virtual EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getLoggerQueueInterface() = 0;

            // Returns Logger Pending Queue class singleton interface
            virtual EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getLoggerPendingQueueInterface() = 0;

            // Returns HTTP Queue class singleton interface
            virtual EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getHTTPQueueInterface() = 0;

        protected:
            NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE m_nwInterface = nullptr;
#ifdef _ENABLE_DB_
            DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE m_dbConnector = nullptr;
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE m_dbQueueInterface = nullptr;
#endif
            HTTP_SERVICE::S_PTR_HTTP_UTILITY m_HttpUtility = nullptr;
            HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER m_HttpSesssionManager = nullptr;
            LOGGER_SERVICE::S_PTR_LOGGER m_Logger = nullptr;
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE m_loggerQueueInterface = nullptr;
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE m_loggerPendingQueueInterface = nullptr;
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE m_HTTPQueueInterface = nullptr;

        private:
            ConsoleAppInterface(const ConsoleAppInterface&) = delete;
            ConsoleAppInterface& operator=(const ConsoleAppInterface&) = delete;
            ConsoleAppInterface(const ConsoleAppInterface&&) = delete;
            ConsoleAppInterface& operator=(const ConsoleAppInterface&&) = delete;
    };

    typedef std::shared_ptr<FRAMEWORK::ConsoleAppInterface> S_PTR_CONSOLEAPPINTERFACE;
}

#endif