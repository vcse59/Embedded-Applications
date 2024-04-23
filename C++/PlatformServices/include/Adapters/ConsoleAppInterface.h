/**
 * @file ConsoleAppInterface.h
 *
 * @brief This file declares the interface for Console application
 *
 * @author Vivek Yadav
 * Contact: v.cse59@gmail.com
 *
 */

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

// Include header files
#include "Interfaces/DataBaseTableInterface.h"
#include "Interfaces/DataBaseConnectorInterface.h"
#include "Interfaces/NetworkClassInterface.h"
#include "Modules/TCPService/TCPServer.h"
#include "Modules/TCPService/TCPClient.h"
#include "Containers/Queue.h"

namespace FRAMEWORK
{
    class ConsoleAppInterface
    {
        public:

            // Destructor
            virtual ~ConsoleAppInterface(){}

            // Returns NetworkClassInterface class singleton instance
            virtual NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPServer() = 0;

            // Returns NetworkClassInterface class singleton instance
            virtual NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE getTCPClient() = 0;

            // Returns DataBaseTableInterface class singleton instance
            virtual DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE getDataBaseTable() = 0;

            // Returns QueueContainer class singleton instance
            virtual Storage::QueueContainer<COMMON_DEFINITIONS::SingleLLNode> &getQueue() = 0;
            
            // Returns DataBaseConnectorInterface class singleton instance
            virtual DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE getDBInstance() = 0;

            // Returns SingleLinkedList class singleton instance
            virtual Storage::SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode> &getSingleLinkedList() = 0;

        protected:

            NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE m_nwInterface = NULL;
            DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE m_dbTableInterface = NULL;
            Storage::QueueContainer<COMMON_DEFINITIONS::SingleLLNode>* m_queueInterface = NULL;
            DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE m_dbConnector = NULL;
            Storage::SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode> *m_linkedList = NULL;
    };
    typedef std::shared_ptr<FRAMEWORK::ConsoleAppInterface> S_PTR_CONSOLEAPPINTERFACE;
}

#endif