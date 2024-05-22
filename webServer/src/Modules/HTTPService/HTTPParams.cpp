#include <sstream>
#include <algorithm>
#include <fstream>

#include "Modules/HTTPService/HTTPParams.h"
#include "Modules/ConsoleMain.h"

using namespace HTTP_SERVICE;

HttpParams::HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string httpRequest)
:m_logger(logger),
 m_HttpRequest(httpRequest)
{
    parse();
}

void HttpParams::generateCookieIfRequired()
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    std::string cookieID = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    if (cookieID.length() == 0)
    {
        // Cookie is sent in request
        // Create a new one
        cookieID = httpUtility->generateSessionID();

        std::string requestID = cookieID;

        // Save the sessionId
        // for future messaging
        m_HeaderInfo.insert(std::pair<HTTP_SERVICE::eHEADER_FIELD, std::string>(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE, requestID));
    }
}

HttpParams::~HttpParams()
{

}

std::string HttpParams::generateLogin(std::string errorMessage) {
    
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    // Read index.html content
    std::string index_html_content = httpUtility->readIndexHtml("webFiles/HTML/index.html");    
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::string msgToReplace = "ERROR_MESSAGE";
    index_html_content.replace(index_html_content.find(msgToReplace), msgToReplace.length(), errorMessage);

    std::stringstream response;
    if(errorMessage.length() > 0)
        response << "HTTP/1.1 401 UnAuthorized\r\n";
    else
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
    std::string index_html_content = httpUtility->readIndexHtml("webFiles/HTML/home.html");    
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::string msgToReplace = "MESSAGE";
    std::string body = "<div class=\"container\">   \
        <h2>About Me</h2>   \
        <p>I'm a passionate developer who likes to explore new tech</p> \
        <p>Please reach me out at <b>v.cse59@gmail.com</b></p> \
        </div>";
    index_html_content.replace(index_html_content.find(msgToReplace), msgToReplace.length(), body);

    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Security-Policy: default-src 'self'\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie
    response << "Content-Length: " << index_html_content.size() << "\r\n\r\n";
    response << index_html_content;

    //std::cout << "RESPONSE : " << response.str() << std::endl;
    return response.str();
}

std::string HttpParams::generateRedirect(std::string& hostURL, std::string newURL)
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    // Read index.html content
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::stringstream response;
    response << "HTTP/1.1 301 Found\r\n";
    response << "Location: " << hostURL << newURL << "\r\n\r\n";
    response << "Content-Security-Policy: default-src 'self'\r\n";
    response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie
    return response.str();
}

void HttpParams::parse()
{
    std::stringstream streamString(m_HttpRequest);
    std::string line;
    std::string  httpString = "HTTP/";

    while(getline(streamString, line))
    {
        if (line == "\n")
            continue;

        // Remove leading and trailing whitespaces from header value
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);   

        // parse method and resouce url
        if (line.find(httpString) != std::string::npos){
            int index = 0;
            for (auto it = HTTP_METHOD.begin(); it != HTTP_METHOD.end(); it++, index++){
                if (line.find(it->first) != std::string::npos)
                {
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

            // Handle invalid method and redirect to login page
            auto headerIter = m_HeaderInfo.find(HTTP_SERVICE::eHEADER_FIELD::HEADER_METHOD);
            if (headerIter == m_HeaderInfo.end())
            {
                LOGGER(m_logger) << "HTTP Method is missing in request" << std::endl; 
                m_HttpError = HTTP_SERVICE::eHTTP_ERROR::INVALID_HTTP_METHOD;
                LOGGER(m_logger) << "Incorrect HTTP Method detected...Falling back to login page with GET Method" << std::endl;
                m_HeaderInfo.insert(std::pair<HTTP_SERVICE::eHEADER_FIELD, std::string>(HTTP_SERVICE::eHEADER_FIELD::HEADER_METHOD, "GET"));
                headerIter = m_HeaderInfo.find(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL);
                std::string defaultResourceURL = "/login";
                if (headerIter != m_HeaderInfo.end())
                {
                    headerIter->second = defaultResourceURL;
                }else{
                    m_HeaderInfo.insert(std::pair<HTTP_SERVICE::eHEADER_FIELD, std::string>(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL, defaultResourceURL));
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
            continue;
        }
    
        m_HttpRequestBody += removeSpecialCharacters(line);
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

std::string HttpParams::generateErrorResponse(std::string errorMessage)
{
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::stringstream response;
    response << "HTTP/1.1 401 UnAuthorized OK\r\n";
    response << "Content-Security-Policy: default-src 'self'\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie
    response << "Content-Length: " << errorMessage.size() << "\r\n\r\n";
    response << errorMessage << "\r\n";
    
    return response.str();
}

std::string HttpParams::generateCSSResponse(std::string resourceURL)
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();
    
    // Parse the file name
    string fileName; 
    std::size_t pos = resourceURL.find_first_not_of('/');
    if (pos != std::string::npos)
    {
        fileName = resourceURL.substr(pos, resourceURL.size() - pos);
        LOGGER(m_logger) << "File name : " << fileName << std::endl;
    }

    // Read css content
    std::string index_html_content = httpUtility->readIndexHtml("webFiles/CSS/" + fileName);    
    std::string sessiondId = getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);

    std::stringstream response;
    if (index_html_content.length() == 0)
    {
        response << "HTTP/1.1 404 Not Found\r\n";
        response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie

    }
    else{
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/css\r\n";
        response << "Set-Cookie: " << sessiondId << "\r\n"; // Embed session ID in cookie
        response << "Content-Length: " << index_html_content.size() << "\r\n\r\n";
        response << index_html_content;
    }

    //std::cout << "RESPONSE : " << response.str() << std::endl;
    return response.str();
}
