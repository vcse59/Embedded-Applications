/**
 * @file ConsoleMain.h
 *
 * @brief This file declares consoleMain methods
 *
 * @author Vivek Yadav
 * Contact: v.cse59@gmail.com
 *
 */

#ifndef __CONSOLE_MAIN_H__
#define __CONSOLE_MAIN_H__

// Include header files
#include "Adapters/ConsoleAppInterface.h"
#include "Modules/TCPService/TCPServer.h"
#include "Modules/TCPService/TCPClient.h"
#include "Modules/Logger/Logger.h"
#include "Modules/EventMessage/DBEventQueue.h"
#include "Modules/EventMessage/LoggerEventQueue.h"
#include "Modules/EventMessage/HTTPEventQueue.h"
#include "Modules/DataBaseTable.h"
#include "Modules/MySQLConnector.h"
#include "Modules/Logger/ConsoleWriter.h"
#include "Modules/Logger/FileWriter.h"

namespace FRAMEWORK
{
    class ConsoleMain : public ConsoleAppInterface
    {
        public:
            // Constructor
            ConsoleMain(){}

            // Destructor
            ~ConsoleMain();

            // Initialize the application
            static COMMON_DEFINITIONS::eSTATUS Initialize()
            {
    
                COMMON_DEFINITIONS::eSTATUS status = COMMON_DEFINITIONS::eSTATUS::SUCCESS;
                std::shared_ptr<FRAMEWORK::ConsoleAppInterface> consoleApp = getConsoleAppInterface();
                LOGGER_SERVICE::S_PTR_LOGGER logger = consoleApp->getLogger();
                NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE nwServerIntf = consoleApp->getTCPServer();
                DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE tableIntf = consoleApp->getDataBaseTable();
                DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE dbIntf = consoleApp->getDBInstance();
                HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();
                HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER httpSessionManager = consoleApp->getHTTPSessionManager();
                EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE dbQueueInterface = consoleApp->getDBQueueInterface();
                EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE loggerQueueInterface = consoleApp->getLoggerQueueInterface();
                EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE httpQueueInterface = consoleApp->getHTTPQueueInterface();

                status = dbQueueInterface->initializeQueue();

                // Initialize the queue for db
                if (status != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
                {
                    LOGGER(logger) << "Failed to initialize DB queue" << std::endl;
                    return status;
                }

                status = loggerQueueInterface->initializeQueue();

                // Initialize the queue for logger
                if (status != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
                {
                    LOGGER(logger) << "Failed to initialize logger queue" << std::endl;
                    return status;
                }

                status = httpQueueInterface->initializeQueue();

                // Initialize the queue for db
                if (status != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
                {
                    LOGGER(logger) << "Failed to initialize HTTPEvent queue" << std::endl;
                    return status;
                }

                status = dbIntf->initializeDB();

                // Initialize the Database
                if (status != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
                {
                    LOGGER(logger) << "Initializing DB is failed...Exiting the program" << std::endl;
                    return status;
                }

                // Load the Access Token Data
                if (httpSessionManager->init() != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
                {
                    LOGGER(logger) << "Failed to load access Token Data" << std::endl;
                    return COMMON_DEFINITIONS::eSTATUS::ERROR;
                }
                return status;
            }

            // Returns NetworkClassInterface class singleton instance
            NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPServer() override;
            
            // Returns DataBaseTableInterface class singleton instance
            DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE getDataBaseTable() override;

            // Returns DataBaseConnectorInterface class singleton instance
            DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE getDBInstance() override;

            // Returns HttpParser class singleton instance
            HTTP_SERVICE::S_PTR_HTTP_UTILITY &getHTTPUtility() override;

            // Returns HttpSessionManager class singleton instance
            HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER &getHTTPSessionManager() override;

            // Returns HttpSessionManager class singleton instance
            LOGGER_SERVICE::S_PTR_LOGGER &getLogger() override;

            // Returns DB Queue class singleton interface
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getDBQueueInterface() override;

            // Returns Logger Queue class singleton interface
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getLoggerQueueInterface() override;

            // Returns HTTP Queue class singleton interface
            EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &getHTTPQueueInterface() override;

            // Returns S_PTR_CONSOLEAPPINTERFACE class singleton instance
            static std::shared_ptr<FRAMEWORK::ConsoleAppInterface> getConsoleAppInterface()
            {
                if (m_Interface == NULL)
                {
                    m_Interface = std::make_shared<FRAMEWORK::ConsoleMain>();
                }

                return m_Interface;
            }
            static std::shared_ptr<FRAMEWORK::ConsoleAppInterface> m_Interface;
        
        private:
            ConsoleMain(const ConsoleMain&) = delete;
            ConsoleMain& operator=(const ConsoleMain&) = delete;
            ConsoleMain(const ConsoleMain&&) = delete;
            ConsoleMain& operator=(const ConsoleMain&&) = delete;
    };

    typedef std::shared_ptr<FRAMEWORK::ConsoleMain> S_PTR_CONSOLEMAIN;
}
#endif