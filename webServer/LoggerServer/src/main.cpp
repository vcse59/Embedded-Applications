#include "LoggerServer.h"

int main(int argc, char* argv[])
{
    std::shared_ptr<LOGGER_SERVICE::LoggerServer> logServer = std::make_shared<LOGGER_SERVICE::LoggerServer>();
    logServer->startServer();
    return 0;
}