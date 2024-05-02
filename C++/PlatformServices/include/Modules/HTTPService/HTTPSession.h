#ifndef __HTTP_SESSION_H__
#define __HTTP_SESSION_H_

#include <chrono>

#include "CommonClasses/CommonDefinitions.h"
#include "Modules/Logger/Logger.h"
#include "Modules/HTTPService/HTTPParams.h"

namespace HTTP_SERVICE
{
    class HttpSession
    {
        public:
            HttpSession(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string sessionId);
            ~HttpSession();

            bool isSessionAlive();
            void setSessionTimeout(std::chrono::microseconds microSeconds);
            std::chrono::microseconds getSessionTimeout() const;
            void processHTTPMessage(HTTP_SERVICE::HttpParams& httpParams);

        private:

            std::string m_SessionId;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HttpSession(const HttpSession&) = delete;
            HttpSession& operator=(const HttpSession&) = delete;
            HttpSession(const HttpSession&&) = delete;
            HttpSession& operator=(const HttpSession&&) = delete;
    };

    typedef std::shared_ptr<HTTP_SERVICE::HttpSession> S_PTR_HTTP_SESSION;
}

#endif