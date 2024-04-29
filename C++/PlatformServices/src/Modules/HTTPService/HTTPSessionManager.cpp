#include "Modules/HTTPService/HTTPSessionManager.h"
#include "Modules/Logger/Logger.h"

using namespace HTTP_SERVICE;

HttpSessionManager::HttpSessionManager(LOGGER_SERVICE::S_PTR_LOGGER logger,
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility) 
    :m_logger(logger), m_httpUtility(httpUtility){
}

HttpSessionManager::~HttpSessionManager(){}

COMMON_DEFINITIONS::eHTTP_SESSION_STATUS HttpSessionManager::addSession(std::string sessionId){

    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Entering HttpSessionManager::addSession" << std::endl;
    COMMON_DEFINITIONS::eHTTP_SESSION_STATUS retVal = COMMON_DEFINITIONS::eHTTP_SESSION_STATUS::SESSION_EXISTS;
    if (m_SessionInfo.find(sessionId) == m_SessionInfo.end())
    {
        m_SessionInfo.insert(std::pair<std::string, S_PTR_HTTP_SESSION>(sessionId, std::make_shared<HttpSession>(m_logger, sessionId)));
        retVal = COMMON_DEFINITIONS::eHTTP_SESSION_STATUS::SESSION_ADDED;
    }
    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Exiting HttpSessionManager::addSession" << std::endl;
    return retVal;
}

COMMON_DEFINITIONS::eHTTP_SESSION_STATUS HttpSessionManager::removeSession(std::string sessionId){
    if (m_SessionInfo.find(sessionId) != m_SessionInfo.end())
        m_SessionInfo.erase(sessionId);
    return COMMON_DEFINITIONS::eHTTP_SESSION_STATUS::SESSION_DELETED;
}

COMMON_DEFINITIONS::eHTTP_SESSION_STATUS HttpSessionManager::isValidSession(std::string sessionId){
    std::cout << "SESSION : " << sessionId << std::endl;
    if (m_SessionInfo.find(sessionId) == m_SessionInfo.end()){
        return COMMON_DEFINITIONS::eHTTP_SESSION_STATUS::SESSION_EXPIRED;
    }
    return COMMON_DEFINITIONS::eHTTP_SESSION_STATUS::SESSION_VALID;
}

void HttpSessionManager::processHTTPMessage(std::string message)
{

}
