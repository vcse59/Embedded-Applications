/**
 * @file LoggerServer.h
 *
 * @brief This file declares LoggerServer class to demonstrate
 *        Collecting logs
 *
 * @author Vivek Yadav
 * Contact: v.cse59@gmail.com
 *
 */

#ifndef __LOGGER_SERVER_H__
#define __LOGGER_SERVER_H__

#include <thread>
#include <string>
#include <memory>

#include "CommonClasses/CommonDefinitions.h"

namespace LOGGER_SERVICE
{
    class LoggerServer
    {
        public:
            LoggerServer();
            ~LoggerServer();

            COMMON_DEFINITIONS::eSTATUS startServer();

        private:
            int mServerSocket = -1;
            void handleLoggerClient(int clientSocket);
    };
}

#endif