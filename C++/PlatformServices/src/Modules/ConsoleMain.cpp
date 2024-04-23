#include "Modules/ConsoleMain.h"
#include "CommonClasses/CommonDefinitions.h"
#include "Modules/DataBaseTable.h"
#include "Modules/MySQLConnector.h"

using namespace FRAMEWORK;
using namespace DATABASE_SERVICE;
using namespace Storage;
using namespace NetworkClass;

std::shared_ptr<FRAMEWORK::ConsoleAppInterface> FRAMEWORK::ConsoleMain::m_Interface;

ConsoleMain::~ConsoleMain(){
    }

NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ConsoleMain::getTCPServer()
{
    if (m_nwInterface == nullptr)
    {
        m_nwInterface = std::make_shared<TCPServer>(PORT);
    }

    return m_nwInterface;
}

NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ConsoleMain::getTCPClient()
{
    if (m_nwInterface == nullptr)
    {
        m_nwInterface = std::make_shared<TCPClient>("127.0.0.1", PORT);
    }

    return m_nwInterface;
}

DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE ConsoleMain::getDataBaseTable()
{
    if (m_dbTableInterface == NULL)
    {
        m_dbTableInterface = std::make_shared<DataBaseTable>();
    }
    return m_dbTableInterface;
}

Storage::QueueContainer<COMMON_DEFINITIONS::SingleLLNode> &ConsoleMain::getQueue()
{
    if (m_queueInterface == NULL)
    {
        m_queueInterface = new QueueContainer<COMMON_DEFINITIONS::SingleLLNode>();
    }
    return *m_queueInterface;
}

DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE ConsoleMain::getDBInstance()
{
    if (m_dbConnector == NULL)
    {
        m_dbConnector = std::make_shared<MySQLConnector>();
    }
    return m_dbConnector;
}
Storage::SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode> &ConsoleMain::getSingleLinkedList()
{
    if (m_linkedList == NULL)
    {
        m_linkedList = new SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode>();
    }
    return *m_linkedList;
}
