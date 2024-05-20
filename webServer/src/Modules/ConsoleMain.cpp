#include "Modules/ConsoleMain.h"
#include "CommonClasses/CommonDefinitions.h"

using namespace FRAMEWORK;
using namespace Storage;
using namespace NetworkClass;

std::shared_ptr<FRAMEWORK::ConsoleAppInterface> FRAMEWORK::ConsoleMain::m_Interface;

ConsoleMain::~ConsoleMain(){
    if (m_linkedList != nullptr)
        delete m_linkedList;
    m_linkedList = nullptr;

    if (m_queueInterface != nullptr)
        delete m_queueInterface;
    m_queueInterface = nullptr;
}

NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ConsoleMain::getTCPServer()
{
    if (m_nwInterface == nullptr)
    {
        m_nwInterface = std::make_shared<TCPServer>(getLogger(), PORT);
    }

    return m_nwInterface;
}

NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ConsoleMain::getTCPClient()
{
    if (m_nwInterface == nullptr)
    {
        m_nwInterface = std::make_shared<TCPClient>(getLogger(), "127.0.0.1", PORT);
    }

    return m_nwInterface;
}

Storage::QueueContainer<COMMON_DEFINITIONS::SingleLLNode> &ConsoleMain::getQueue()
{
    if (m_queueInterface == nullptr)
    {
        m_queueInterface = new QueueContainer<COMMON_DEFINITIONS::SingleLLNode>(getLogger());
    }
    return *m_queueInterface;
}

Storage::SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode>& ConsoleMain::getSingleLinkedList()
{
    if (m_linkedList == nullptr)
    {
        m_linkedList = new SingleLinkedList<COMMON_DEFINITIONS::SingleLLNode>(getLogger());
    }
    return *m_linkedList;
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
