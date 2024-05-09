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
    DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE dbConnector = consoleApp->getDBInstance();
    std::string encodedPayload = httpUtility->base64_encode(httpParams.getHTTPRequest());
    std::string sessionID = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE);
    std::string resourceURL = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL);
    std::string httpMethod = httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_METHOD);
    std::string insertQuery = "INSERT INTO " + tableName + " (SESSIONID, HTTPMETHOD, RESOURCEURL, DATARAW)  \
        VALUES (\"" +  sessionID + "\" , \"" + httpMethod + "\", \""  + resourceURL + "\", \"" + encodedPayload + "\")";

    COMMON_DEFINITIONS::eSTATUS status = dbConnector->executeQuery(tableName, insertQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_MANIPULATION);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        std::cout << "Record created successfully" << std::endl;
    }
    else{
        std::cout << "Unable to insert record" << std::endl;
    }

    std::string response;
    switch(HTTP_METHOD[httpMethod])
    {
        case HTTP_SERVICE::eHTTP_METHODS::HTTP_GET:
            response = processGet(httpParams);
            break;

        case HTTP_SERVICE::eHTTP_METHODS::HTTP_POST:
            response = processPost(httpParams);
            break;
        default:
            (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Invalid method type" << std::endl;
    }

    return response;
}

std::string HttpSession::processGet(HTTP_SERVICE::HttpParams& httpParams)
{
    std::cout << "************Processing GET........" << std::endl;
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();
    if (httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL) == "/")
    {
        return httpParams.generateRedirect(m_HostURL, "/login");
    }
    else if (httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL) == "/login")
    {
        return httpParams.generateLogin();
    }
    else if (httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL) == "/home")
    {
        return httpParams.generateHomePage();
    }
    else if (httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL) == "/favicon.ico")
    {
        return httpParams.generateFavicon();
    }
    else {
        return "";
    }

    return "";
}

std::string HttpSession::processPost(HTTP_SERVICE::HttpParams& httpParams)
{
    std::cout << "************Processing POST........" << std::endl;
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();
    if (httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_RESOURCE_URL) == "/login")
    {
        return httpParams.generateRedirect(m_HostURL, "/home");
    }

    return "";
}

std::string HttpSession::processLogin(HTTP_SERVICE::HttpParams& httpParams)
{
    return "";
}
