#ifndef __HTTP_SESSION_MANAGER_H__
#define __HTTP_SESSION_MANAGER_H__

#include <chrono>
#include <unordered_map>

#include "CommonClasses/CommonDefinitions.h"
#include "Modules/HTTPService/HTTPSession.h"
#include "Modules/Logger/Logger.h"
#include "Modules/HTTPService/HTTPParams.h"

namespace HTTP_SERVICE
{
    class HttpSessionManager
    {
        public:
            HttpSessionManager(LOGGER_SERVICE::S_PTR_LOGGER logger);
            ~HttpSessionManager();

            COMMON_DEFINITIONS::eHTTP_SESSION_STATUS addSession(std::string sessionId);
            COMMON_DEFINITIONS::eHTTP_SESSION_STATUS removeSession(std::string sessionId);
            COMMON_DEFINITIONS::eHTTP_SESSION_STATUS isValidSession(std::string sessionId);
            void processHTTPMessage(HTTP_SERVICE::HttpParams& httpParams);
            
        private:

            std::unordered_map<std::string, HTTP_SERVICE::S_PTR_HTTP_SESSION> m_SessionInfo;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HttpSessionManager(const HttpSessionManager&) = delete;
            HttpSessionManager& operator=(const HttpSessionManager&) = delete;
            HttpSessionManager(const HttpSessionManager&&) = delete;
            HttpSessionManager& operator=(const HttpSessionManager&&) = delete;
    };

    typedef std::shared_ptr<HTTP_SERVICE::HttpSessionManager> S_PTR_HTTP_SESSION_MANAGER;
    typedef std::unordered_map<std::string, HTTP_SERVICE::S_PTR_HTTP_SESSION>   HTTP_SESSION_MAP;
}

#endif