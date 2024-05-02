#include <sstream>
#include <algorithm>

#include "Modules/HTTPService/HTTPParams.h"

using namespace HTTP_SERVICE;

HttpParams::HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger, std::string httpResponse)
:m_logger(logger),
 m_HttpResponse(httpResponse)
{
    parseHttpResponse();
}

HttpParams::~HttpParams()
{

}

void HttpParams::parseHttpResponse()
{
    std::istringstream iss(m_HttpResponse);

    std::string line;
    
    if (getline(iss, line))
    {
        if (line.find("POST") != std::string::npos)
        {
            m_Method = HTTP_SERVICE::eHTTP_METHODS::HTTP_POST;
        }
    }

    while(getline(iss, line))
    {
        std::cout << "Line : " << line << std::endl;
        if(line.find("Host") != std::string::npos)
        {
            const char *result = strstr(line.c_str(), ":");
            std::string hostInfo = std::string(result + 1);
            std::string filler;
            std::remove_copy_if(hostInfo.begin(), hostInfo.end(), std::back_inserter(filler) , [](unsigned char c){return std::isspace(c);});
            m_HostIP = filler;
        }

        if(line.find("X-Real-IP") != std::string::npos)
        {
            const char *result = strstr(line.c_str(), ":");
            std::string hostInfo = std::string(result + 1);
            std::string filler;
            std::remove_copy_if(hostInfo.begin(), hostInfo.end(), std::back_inserter(filler) , [](unsigned char c){return std::isspace(c);});
            m_HostIP = filler;
        }
    }
}
