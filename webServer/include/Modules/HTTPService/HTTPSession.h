#ifndef __HTTP_SESSION_H__
#define __HTTP_SESSION_H__

#include <chrono>

#include "CommonClasses/CommonDefinitions.h"
#include "Modules/HTTPService/HTTPParams.h"

namespace HTTP_SERVICE
{
    class HttpSession
    {
        public:
            HttpSession(LOGGER_SERVICE::S_PTR_LOGGER logger
                        , std::string sessionId
                        , std::string hostURL);
            ~HttpSession();

            bool isSessionAlive();
            void setSessionTimeout(std::chrono::microseconds microSeconds);
            std::chrono::microseconds getSessionTimeout() const;
            std::string processHTTPMessage(HTTP_SERVICE::HttpParams& httpParams);

        private:

            std::string m_SessionId;
            std::string m_UserId;
            std::string m_HostURL;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HttpSession(const HttpSession&) = delete;
            HttpSession& operator=(const HttpSession&) = delete;
            HttpSession(const HttpSession&&) = delete;
            HttpSession& operator=(const HttpSession&&) = delete;

            std::string processGet(HTTP_SERVICE::HttpParams& httpParams);
            std::string processPost(HTTP_SERVICE::HttpParams& httpParams);
            COMMON_DEFINITIONS::eSTATUS processLogin(HTTP_SERVICE::HttpParams& httpParams);
    };

    typedef std::shared_ptr<HTTP_SERVICE::HttpSession> S_PTR_HTTP_SESSION;
}

#endif