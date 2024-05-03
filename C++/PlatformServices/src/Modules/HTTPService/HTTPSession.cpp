#include "Modules/HTTPService/HTTPUtility.h"
#include "Modules/ConsoleMain.h"
#include "CommonClasses/CommonDefinitions.h"

using namespace HTTP_SERVICE;

HttpSession::HttpSession(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string sessionId){
    m_logger    = logger;
    m_SessionId = sessionId;
}
HttpSession::~HttpSession(){}

bool HttpSession::isSessionAlive(){
    return true;
}

void HttpSession::setSessionTimeout(std::chrono::microseconds microSeconds){}

std::chrono::microseconds HttpSession::getSessionTimeout() const{
    return std::chrono::microseconds(0);
}

void HttpSession::processHTTPMessage(HTTP_SERVICE::HttpParams& httpParams)
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    HTTP_SERVICE::S_PTR_HTTP_UTILITY httpUtility = consoleApp->getHTTPUtility();

    std::string tableName = "sampleTable";
    DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE dbConnector = consoleApp->getDBInstance();
    std::string encodedPayload = httpUtility->base64_encode(httpParams.getHTTPRequest());
    std::string insertQuery = "INSERT INTO " + tableName + " (SessionID ,Payload) VALUES (\"" + httpParams.getParams(HTTP_SERVICE::eHEADER_FIELD::HEADER_COOKIE) + "\" , \"" + encodedPayload + "\")";
    COMMON_DEFINITIONS::eSTATUS status = dbConnector->executeQuery(tableName, insertQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_MANIPULATION);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        std::cout << "Record created successfully" << std::endl;
    }
    else{
        std::cout << "Unable to insert record" << std::endl;
    }
}