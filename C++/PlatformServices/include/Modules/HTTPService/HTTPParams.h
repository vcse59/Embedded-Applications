#ifndef __HTTP_PARAMS_H__
#define __HTTP_PARAMS_H__

#include <string>
#include "Modules/HTTPService/HTTPDefinitions.h"
#include "Modules/Logger/Logger.h"

namespace HTTP_SERVICE
{
    class HttpParams
    {
        public:
            HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string httpResponse);
            ~HttpParams();

            void parseHttpResponse();
            HTTP_SERVICE::eHTTP_METHODS getHTTPMethods() const
            {
                return m_Method;
            }

            std::string getHostIP() const 
            {
                return m_HostIP;
            }

            std::string getHTTPResponse() const
            {
                return m_HttpResponse;
            }

        private:
            std::string m_HttpResponse;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HTTP_SERVICE::eHTTP_METHODS m_Method = HTTP_SERVICE::eHTTP_METHODS::HTTP_UNKNOWN;
            HTTP_SERVICE::eCONTENT_TYPE m_ContentType = HTTP_SERVICE::eCONTENT_TYPE::APPLICATION_UNKNOWN;
            std::string m_SessonID;
            std::string m_HostIP;
            HttpParams(const HttpParams&) = delete;
            HttpParams& operator=(const HttpParams&) = delete;
            HttpParams(const HttpParams&&) = delete;
            HttpParams& operator=(const HttpParams&&) = delete;
    };
}

#endif