#ifndef __HTTP_PARAMS_H__
#define __HTTP_PARAMS_H__

#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <random>
#include <sstream>
#include <unordered_map>

#include "Modules/HTTPService/HTTPDefinitions.h"
#include "Modules/Logger/Logger.h"

namespace HTTP_SERVICE
{
    class HttpParams
    {
        public:
            HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string httpRequest);
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

            std::string getError() const
            {
                return HTTP_SERVICE::HTTP_ERROR[m_HttpError];
            }

            void parse();
            std::string generateLogin(std::string errorMessage = "");
            std::string generateRedirect(std::string& hostURL, std::string newURL);
            std::string generateHomePage();
            std::string generateFavicon();
            std::string generateErrorResponse(std::string errorMessage);
            std::string generateCSSResponse(std::string cssFileName);
            std::string getHttpRequestBody() const
            {
                return m_HttpRequestBody;
            }

            void removeSpaces(std::string& str)
            {
                str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
            }

            std::string getUserID() const{
                return m_UserId;
            }

        private:
            // Function to remove special characters from a string
            std::string removeSpecialCharacters(const std::string& input) {
                std::string output;
                std::remove_copy_if(input.begin(), input.end(),
                                    std::back_inserter(output),
                                    [](char c) {
                                        // Define what characters to remove
                                        return !(c >= 32 && c <= 126);
                                    });
                return output;
            }
            std::string m_HttpRequest;
            std::string m_HttpRequestBody;
            HTTP_SERVICE::eHTTP_ERROR m_HttpError = HTTP_SERVICE::eHTTP_ERROR::NO_ERROR;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HTTP_SERVICE::eCONTENT_TYPE m_ContentType = HTTP_SERVICE::eCONTENT_TYPE::APPLICATION_UNKNOWN;
            std::string m_HostIP;
            std::string m_UserId;
            std::unordered_map<HTTP_SERVICE::eHEADER_FIELD, std::string> m_HeaderInfo;
            HttpParams(const HttpParams&) = delete;
            HttpParams& operator=(const HttpParams&) = delete;
            HttpParams(const HttpParams&&) = delete;
            HttpParams& operator=(const HttpParams&&) = delete;
            void generateCookieIfRequired();
    };
}

#endif