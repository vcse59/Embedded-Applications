#include <sstream>
#include <algorithm>

#include "Modules/HTTPService/HTTPParams.h"

using namespace HTTP_SERVICE;

HttpParams::HttpParams(LOGGER_SERVICE::S_PTR_LOGGER logger)
:m_logger(logger)
{

}

HttpParams::~HttpParams()
{

}

void HttpParams::parseHttpResponse(std::string httpResponse)
{
    std::istringstream iss(httpResponse);

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
            m_HostIP = std::string(result + 1);
            std::string filler;
            std::remove_copy_if(m_HostIP.begin(), m_HostIP.end(), std::back_inserter(filler) , [](unsigned char c){return std::isspace(c);});
            m_HostIP = filler;
        }
    }
}
