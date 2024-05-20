#include <cstddef>        // std::size_t

#include "Modules/HTTPService/HTTPUtility.h"
#include "Modules/ConsoleMain.h"
#include "CommonClasses/CommonDefinitions.h"

using namespace HTTP_SERVICE;

HttpSession::HttpSession(LOGGER_SERVICE::S_PTR_LOGGER logger
    , std::string sessionId, std::string hostURL){
    m_logger    = logger;
    m_SessionId = sessionId;
    m_HostURL   = "http://" + hostURL;
}
HttpSession::~HttpSession(){}

bool HttpSession::isSessionAlive(){
    return true;
}

void HttpSession::setSessionTimeout(std::chrono::microseconds microSeconds){}

std::chrono::microseconds HttpSession::getSessionTimeout() const{
    return std::chrono::microseconds(0);
}

std::string HttpSession::processHTTPMessage(HTTP_SERVICE::HttpParams& httpParams)
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    std::string tableName = "sampleTable";
    std::string encodedPayload = httpUtility->base64_encode(httpParams.getHTTPRequest());
    std::string sessionID = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);
    std::string resourceURL = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL);
    std::string httpMethod = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_METHOD);
    std::string httpError = httpParams.getError();

    std::string response;
    LOGGER(m_logger) << "Processing Request : " << httpMethod << std::endl;
    LOGGER(m_logger) << "Processing Resource URL : " << resourceURL << std::endl;

    switch(HTTP_METHOD[httpMethod])
    {
        case HTTP_SERVICE::eHTTP_METHODS::HTTP_GET:
            response = processGet(httpParams);
            break;

        case HTTP_SERVICE::eHTTP_METHODS::HTTP_POST:
            response = processPost(httpParams);
            break;
        default:
            LOGGER(m_logger) << "Invalid method type" << std::endl;
    }

    return response;
}

std::string HttpSession::processGet(HTTP_SERVICE::HttpParams& httpParams)
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();
    std::string resourceURL = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL);
    std::string response;
    if (resourceURL == "/")
    {
        response = httpParams.generateRedirect(m_HostURL, "/login");
    }
    else if (resourceURL == "/login")
    {
        response = httpParams.generateLogin();
    }
    else if (resourceURL == "/home")
    {
        response = httpParams.generateHomePage();
    }
    else if (resourceURL == "/favicon.ico")
    {
        response = httpParams.generateFavicon();
    }
    else if (resourceURL.find(".css") != std::string::npos)
    {
        response = httpParams.generateCSSResponse(resourceURL);
    }
    else {
    }

    return response;
}

std::string HttpSession::processPost(HTTP_SERVICE::HttpParams& httpParams)
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();
    if (httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL) == "/login")
    {
        if (processLogin(httpParams) == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
            return httpParams.generateHomePage();
    }

    return httpParams.generateLogin("<span>Invalid credential</span>");
}

COMMON_DEFINITIONS::eSTATUS HttpSession::processLogin(HTTP_SERVICE::HttpParams& httpParams)
{
    std::string requestBody = httpParams.getHttpRequestBody();
    if (requestBody.length() == 0)
        return COMMON_DEFINITIONS::eSTATUS::ERROR;

    std::string username;
    std::string password;

    std::size_t pos = requestBody.find_first_of('&');
    std::string usernameString = requestBody.substr(0, pos);
    std::string passwordString = requestBody.substr(pos, requestBody.length() - pos);

    username = usernameString.substr(usernameString.find_first_of('=') + 1, usernameString.length() - usernameString.find_first_of('='));
    password = passwordString.substr(passwordString.find_first_of('=') + 1, passwordString.length() - passwordString.find_first_of('='));
    
    if (username == "vivek" && password == "vivek")
        return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
    else
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
}
