#ifndef __HTTP_UTILITY_H__
#define __HTTP_UTILITY_H__

#include <unordered_map>

#include "CommonClasses/CommonDefinitions.h"
#include "JsonModule/JsonItem.h"
#include "Modules/Logger/Logger.h"

namespace HTTP_SERVICE
{
    class HTTPUtility{
        public:

            HTTPUtility(LOGGER_SERVICE::S_PTR_LOGGER logger):m_logger(logger){}
            ~HTTPUtility(){}
            std::string parseHttpResponse(const char* httpResponse);
            std::string readIndexHtml(const std::string& filename);
            std::string generateHttpResponse(const std::string& content, const std::string& sessionId);
            std::string generateSessionID();
            std::unordered_map<std::string, std::string> parseHttpHeaders(const std::string& headers, std::string& sessionId);

        private:
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            HTTPUtility(const HTTPUtility&) = delete;
            HTTPUtility& operator=(const HTTPUtility&) = delete;
            HTTPUtility(const HTTPUtility&&) = delete;
            HTTPUtility& operator=(const HTTPUtility&&) = delete;
    };

    typedef std::shared_ptr<HTTP_SERVICE::HTTPUtility> S_PTR_HTTP_UTILITY;
} // namespace HTTP_SERVICE

#endif