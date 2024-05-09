#include "Modules/HTTPService/HTTPSessionManager.h"
#include "Modules/Logger/Logger.h"
#include "Modules/ConsoleMain.h"

using namespace HTTP_SERVICE;

HttpSessionManager::HttpSessionManager(LOGGER_SERVICE::S_PTR_LOGGER logger)
:m_logger(logger){
}

HttpSessionManager::~HttpSessionManager(){}

COMMON_DEFINITIONS::eHTTP_SESSION_STATUS HttpSessionManager::addSession(std::string sessionId){

    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Entering HttpSessionManager::addSession" << std::endl;
    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Exiting HttpSessionManager::addSession" << std::endl;
    return COMMON_DEFINITIONS::eHTTP_SESSION_STATUS::SESSION_ADDED;
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

std::string HttpSessionManager::processHTTPMessage(HTTP_SERVICE::HttpParams& httpParams)
{
    std::string sessionId = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);
    std::string hostUrl = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_HOST);
    LOGGER_SERVICE::Logger* logger = m_logger.get();
    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Session Id Parsed : " << sessionId << std::endl;

    HTTP_SESSION_MAP::iterator it = m_SessionInfo.find(sessionId);
    HTTP_SERVICE::S_PTR_HTTP_SESSION sessionObject = nullptr;
    if (it != m_SessionInfo.end()){
        (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Session found for " << sessionId << std::endl;
        sessionObject = it->second;
    }else{
        (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Creating new session for " << sessionId << std::endl;
        sessionObject = std::make_shared<HttpSession>(m_logger, sessionId, hostUrl);
        m_SessionInfo.insert(std::pair<std::string, S_PTR_HTTP_SESSION>(sessionId, sessionObject));
    }
    return sessionObject->processHTTPMessage(httpParams);
}
