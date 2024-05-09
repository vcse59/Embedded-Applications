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

std::string HttpParams::generateLogin() {
    
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

std::string HttpParams::generateHomePage() {
    
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    // Read index.html content
    std::string index_html_content = httpUtility->readIndexHtml("webFiles/HomePage.html");    
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Security-Policy: default-src 'self'\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie
    response << "Content-Length: " << index_html_content.size() << "\r\n\r\n";
    response << index_html_content;

    std::cout << "RESPONSE : " << response.str() << std::endl;
    return response.str();
}

std::string HttpParams::generateRedirect(std::string& hostURL, std::string newURL)
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    std::cout << "*********HOST URL : " << hostURL << std::endl;
    // Read index.html content
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::stringstream response;
    response << "HTTP/1.1 301 Found\r\n";
    response << "Location: " << hostURL << newURL << "\r\n\r\n";
    response << "Content-Security-Policy: default-src 'self'\r\n";
    response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie

    std::cout << "REDIRECT - RESPONSE : " << response.str() << std::endl;
    return response.str();
}

void HttpParams::parse()
{
    std::stringstream streamString(m_HttpRequest);
    std::string line;
    std::string  httpString = "HTTP/";

    while(getline(streamString, line))
    {
        // Remove leading and trailing whitespaces from header value
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);   
        (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "DATA : " << line << std::endl;

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

            headerFieldName.erase(0, headerFieldName.find_first_not_of(" \t\n\r"));
            headerFieldName.erase(headerFieldName.find_last_not_of(" \t\n\r") + 1); 
            headerFieldValue.erase(0, headerFieldValue.find_first_not_of(" \t\n\r"));
            headerFieldValue.erase(headerFieldValue.find_last_not_of(" \t\n\r") + 1); 

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

std::string HttpParams::generateFavicon()
{
    std::stringstream response;
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    response << "HTTP/1.1 200 OK\r\n";
    response << "Connection: close\r\n"; // Add a newline after Connection header
    response << "\r\n"; // Add an empty line to separate headers from body
    
    return response.str();
}
