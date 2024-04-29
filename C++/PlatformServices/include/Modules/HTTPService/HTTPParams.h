#ifndef __HTTP_PARAMS_H__
#define __HTTP_PARAMS_H__

#include <string>

#include "Modules/ConsoleMain.h"
#include "Modules/HTTPService/HTTPDefinitions.h"

namespace HTTP_SERVICE
{
    class HttpParams
    {
        public:
            HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger);
            ~HttpParams();

            void parseHttpResponse(std::string httpResponse);
            HTTP_SERVICE::eHTTP_METHODS getHTTPMethods() const
            {
                return m_Method;
            }

            std::string getHostIP() const 
            {
                return m_HostIP;
            }

        private:
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HTTP_SERVICE::eHTTP_METHODS m_Method = HTTP_SERVICE::eHTTP_METHODS::HTTP_UNKNOWN;
            HTTP_SERVICE::eCONTENT_TYPE m_ContentType = HTTP_SERVICE::eCONTENT_TYPE::APPLICATION_UNKNOWN;
            std::string m_SessonID;
            std::string m_HostIP;            
    };
}

#endif