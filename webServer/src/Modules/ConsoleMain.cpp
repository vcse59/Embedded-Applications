#include "Modules/ConsoleMain.h"

using namespace FRAMEWORK;
using namespace DATABASE_SERVICE;
using namespace NetworkClass;

std::shared_ptr<FRAMEWORK::ConsoleAppInterface> FRAMEWORK::ConsoleMain::m_Interface;

ConsoleMain::~ConsoleMain(){
}

NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ConsoleMain::getTCPServer()
{
    if (m_nwInterface == nullptr)
    {
        m_nwInterface = std::make_shared<TCPServer>(getLogger(), COMMON_DEFINITIONS::WEB_SERVER_PORT);
    }

    return m_nwInterface;
}

DATABASE_SERVICE::S_PTR_DATABASE_TABLE_INTERFACE ConsoleMain::getDataBaseTable()
{
    if (m_dbTableInterface == nullptr)
    {
        m_dbTableInterface = std::make_shared<DataBaseTable>(getLogger());
    }
    return m_dbTableInterface;
}

DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE ConsoleMain::getDBInstance()
{
    if (m_dbConnector == nullptr)
    {
        m_dbConnector = std::make_shared<MySQLConnector>(getLogger());
    }
    return m_dbConnector;
}

EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &ConsoleMain::getDBQueueInterface()
{
    if (m_dbQueueInterface == nullptr)
    {
        m_dbQueueInterface = std::make_shared<EVENT_MESSAGE::DBEventQueue>();
    }

    return m_dbQueueInterface;
}

HTTP_SERVICE::S_PTR_HTTP_UTILITY& ConsoleMain::getHTTPUtility()
{
    if (m_HttpUtility == nullptr){
        m_HttpUtility = std::make_shared<HTTP_SERVICE::HTTPUtility>(getLogger());
    }
    return m_HttpUtility;
}

HTTP_SERVICE::S_PTR_HTTP_SESSION_MANAGER& ConsoleMain::getHTTPSessionManager()
{
    if (m_HttpSesssionManager == nullptr){
        m_HttpSesssionManager = std::make_shared<HTTP_SERVICE::HttpSessionManager>(getLogger());
    }
    return m_HttpSesssionManager;

}

LOGGER_SERVICE::S_PTR_LOGGER& ConsoleMain::getLogger() 
{
    if (m_Logger == nullptr)
    {
        m_Logger = std::make_shared<LOGGER_SERVICE::LoggerStream>(std::make_shared<LOGGER_SERVICE::RemoteWriter>(COMMON_DEFINITIONS::LOG_SERVER_IP, COMMON_DEFINITIONS::LOG_SERVER_PORT), LOGGER_SERVICE::eLOG_LEVEL_ENUM::ALL_LOG);
    }
    return m_Logger;
}

EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &ConsoleMain::getLoggerQueueInterface()
{
    if (m_loggerQueueInterface == nullptr)
    {
        m_loggerQueueInterface = std::make_shared<EVENT_MESSAGE::LoggerEventQueue>();
    }

    return m_loggerQueueInterface;
}

EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &ConsoleMain::getLoggerPendingQueueInterface()
{
    if (m_loggerPendingQueueInterface == nullptr)
    {
        m_loggerPendingQueueInterface = std::make_shared<EVENT_MESSAGE::LoggerEventQueue>();
    }

    return m_loggerPendingQueueInterface;
}

EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE &ConsoleMain::getHTTPQueueInterface()
{
    if (m_HTTPQueueInterface == nullptr)
    {
        m_HTTPQueueInterface = std::make_shared<EVENT_MESSAGE::HTTPEventQueue>();
    }

    return m_HTTPQueueInterface;
}