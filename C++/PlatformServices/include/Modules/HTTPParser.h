#ifndef __HTTP_PARSER_H__
#define __HTTP_PARSER_H__

#include "CommonClasses/CommonDefinitions.h"
#include "JsonModule/JsonItem.h"

namespace HTTP_SERVICE
{
    class HTTPParser{
        public:
            std::string parseHttpResponse(const char* httpResponse);
            std::string readIndexHtml(const std::string& filename);
            std::string generateHttpResponse(const std::string& content);
    };

    typedef std::shared_ptr<HTTP_SERVICE::HTTPParser> S_PTR_HTTP_PARSER;
} // namespace HTTP_SERVICE

#endif