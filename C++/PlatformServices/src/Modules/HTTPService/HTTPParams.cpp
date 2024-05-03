#include <sstream>
#include <algorithm>
#include <fstream>

#include "Modules/HTTPService/HTTPParams.h"
#include "Modules/ConsoleMain.h"

using namespace HTTP_SERVICE;

HttpParams::HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string httpResponse)
:m_logger(logger),
 m_HttpRequest(httpResponse)
{
    parse();
}

void HttpParams::generateCookieIfRequired()
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);
    if (sessiondId.length() == 0)
    {
        // Cookie is sent in request
        // Create a new one
        sessiondId = httpUtility->generateSessionID();

        // Save the sessionId
        // for future messaging
        m_HeaderInfo.insert(std::pair<HTTP_SERVICE::eHEADER_FIELD, std::string>(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE, sessiondId));
    }
}

HttpParams::~HttpParams()
{

}

std::string HttpParams::generateHttpResponse() {
    
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    // Read index.html content
    std::string index_html_content = httpUtility->readIndexHtml("webFiles/index.html");    
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie
    response << "Content-Length: " << index_html_content.size() << "\r\n\r\n";
    response << index_html_content;

    //std::cout << "RESPONSE : " << response.str() << std::endl;
    return response.str();
}


void HttpParams::parse()
{
    std::stringstream streamString(m_HttpRequest);
    std::string line;
    std::string  httpString = "HTTP/";

    while(getline(streamString, line))
    {
        (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << line << std::endl;
        // Remove leading and trailing whitespaces from header value
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);   

        // parse method and resouce url
        if (line.find(httpString) != std::string::npos){
            int index = 0;
            for (auto it = HTTP_METHOD.begin(); it != HTTP_METHOD.end(); it++, index++){
                if (line.find(it->first) != std::string::npos)
                {
                    m_Method = it->second;
                    m_HeaderInfo.insert(std::pair<HTTP_SERVICE::eHEADER_FIELD, std::string>(HTTP_SERVICE::eHEADER_FIELD::HEADER_METHOD, it->first));

                    // parse the resouce url
                    std::string parsedString = line.substr(it->first.length());
                    size_t pos = parsedString.rfind(httpString);
                    if (pos != std::string::npos){
                        parsedString = parsedString.replace( pos , parsedString.length() - pos, "");
                    }

                    // Remove space it there are any
                    removeSpaces(parsedString);
                    std::cout <<"********" << parsedString << std::endl;
                    if (parsedString.length() > 0)
                        m_HeaderInfo.insert(std::pair<HTTP_SERVICE::eHEADER_FIELD, std::string>(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL, parsedString));
                }
            }
            continue;
        }

        // Parse the header with delimiter :
        size_t pos = std::string::npos;
        if ((pos = line.find_first_of(':')) != std::string::npos)
        {
            std::string headerFieldName = line.substr(0, pos);
            std::string headerFieldValue = line.substr(pos + 1, line.length() - pos);
            auto it = HEADER_STRING.find(headerFieldName);
            if (it != HEADER_STRING.end())
            {
                m_HeaderInfo.insert(std::pair<HTTP_SERVICE::eHEADER_FIELD, std::string>(it->second, headerFieldValue));
            }
        }
    }

    // Generate Cookie if it's not there in request
    generateCookieIfRequired();

}
