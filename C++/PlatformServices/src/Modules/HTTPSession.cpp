#include "Modules/HTTPSession.h"

using namespace HTTP_SERVICE;

HttpSession::HttpSession(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string sessionId){
    m_logger    = logger;
    m_SessionId = sessionId;
}
HttpSession::~HttpSession(){}

bool HttpSession::isSessionAlive(){
    return true;
}

void HttpSession::setSessionTimeout(std::chrono::microseconds microSeconds){}

std::chrono::microseconds HttpSession::getSessionTimeout() const{
    return std::chrono::microseconds(0);
}
