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

namespace FRAMEWORK
{
    class ConsoleMain : public ConsoleAppInterface
    {
        public:
            // Constructor
            ConsoleMain(){}

            // Destructor
            ~ConsoleMain();

            // Returns NetworkClassInterface class singleton instance
            NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPServer() override;

            // Returns NetworkClassInterface class singleton instance
            NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPClient() override;
            
            // Returns DataBaseTableInterface class singleton instance
            DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE getDataBaseTable() override;

            // Returns QueueContainer class singleton instance
            Storage::QueueContainer<COMMON_DEFINITIONS::SingleLLNode>& getQueue() override;

            // Returns DataBaseConnectorInterface class singleton instance
            DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE getDBInstance() override;

            // Returns SingleLinkedList class singleton instance
            Storage::SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode> &getSingleLinkedList() override;

            // Returns HttpParser class singleton instance
            HTTP_SERVICE::S_PTR_HTTP_UTILITY &getHTTPUtility() override;

            // Returns HttpSessionManager class singleton instance
            HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER &getHTTPSessionManager() override;

            // Returns HttpSessionManager class singleton instance
            LOGGER_SERVICE::S_PTR_LOGGER &getLogger() override;

            // Returns S_PTR_CONSOLEAPPINTERFACE class singleton instance
            static S_PTR_CONSOLEAPPINTERFACE getConsoleAppInterface()
            {
                if (m_Interface == NULL)
                {
                    m_Interface = std::make_shared<FRAMEWORK::ConsoleMain>();
                }

                return m_Interface;
            }
            static S_PTR_CONSOLEAPPINTERFACE m_Interface;
        
        private:
            ConsoleMain(const ConsoleMain&) = delete;
            ConsoleMain& operator=(const ConsoleMain&) = delete;
            ConsoleMain(const ConsoleMain&&) = delete;
            ConsoleMain& operator=(const ConsoleMain&&) = delete;
    };

    typedef std::shared_ptr<FRAMEWORK::ConsoleMain> S_PTR_CONSOLEMAIN;
}

#endif