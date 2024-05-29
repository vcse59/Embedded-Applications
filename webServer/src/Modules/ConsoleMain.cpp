#include "Modules/ConsoleMain.h"
#include "CommonClasses/CommonDefinitions.h"
#include "Modules/DataBaseTable.h"
#include "Modules/MySQLConnector.h"

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
        m_nwInterface = std::make_shared<TCPServer>(getLogger(), PORT);
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
        m_Logger = std::make_shared<LOGGER_SERVICE::LoggerStream>(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG);
    }
    return m_Logger;
}
