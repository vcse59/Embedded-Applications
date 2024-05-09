#ifndef __HTTP_PARAMS_H__
#define __HTTP_PARAMS_H__

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Modules/HTTPService/HTTPDefinitions.h"
#include "Modules/Logger/Logger.h"

namespace HTTP_SERVICE
{
    class HttpParams
    {
        public:
            HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string httpResponse);
            ~HttpParams();

            std::string getParams(HTTP_SERVICE::eHEADER_FIELD method) const
            {
                std::string paramVal;
                auto it = m_HeaderInfo.find(method);
                if (it != m_HeaderInfo.end())
                    paramVal = it->second;
                return paramVal;
            }

            std::string getHostIP() const 
            {
                return m_HostIP;
            }

            std::string getHTTPRequest() const
            {
                return m_HttpRequest;
            }

            void parse();
            std::string generateLogin();
            std::string generateRedirect(std::string& hostURL, std::string newURL);
            std::string generateHomePage();
            std::string generateFavicon();

            void removeSpaces(std::string& str)
            {
                str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
            }

        private:
            std::string m_HttpRequest;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HTTP_SERVICE::eHTTP_METHODS m_Method = HTTP_SERVICE::eHTTP_METHODS::HTTP_UNKNOWN;
            HTTP_SERVICE::eCONTENT_TYPE m_ContentType = HTTP_SERVICE::eCONTENT_TYPE::APPLICATION_UNKNOWN;
            std::string m_SessonID;
            std::string m_HostIP;
            std::unordered_map<HTTP_SERVICE::eHEADER_FIELD, std::string> m_HeaderInfo;
            HttpParams(const HttpParams&) = delete;
            HttpParams& operator=(const HttpParams&) = delete;
            HttpParams(const HttpParams&&) = delete;
            HttpParams& operator=(const HttpParams&&) = delete;
            void generateCookieIfRequired();
    };
}

#endif