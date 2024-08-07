#ifndef __REMOTE_WRITER_H__
#define __REMOTE_WRITER_H__

#include "Modules/ConsoleMain.h"

#include <cstdint>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h> // read(), write(), close()
#include <fcntl.h>
#include <assert.h>
#include <iostream>
#include <errno.h>
#include <arpa/inet.h> //close
#include <string.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace LOGGER_SERVICE
{
    class RemoteWriter : public LOGGER_SERVICE::LogWriterInterface
    {
    public:
        RemoteWriter(std::string loggerServiceIP, unsigned int portNumber)
            : RemoteWriter::LogWriterInterface(std::bind(&RemoteWriter::processLogEvents, this)), mPortNumber{portNumber}, mServerIPAddress(loggerServiceIP)
        {
            mRetryThread = std::make_shared<std::thread>(&RemoteWriter::retryConnection, this);
            mRetryThread->detach();

            /*if (connectToRemoteLogServer() != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
            {
                std::unique_lock<std::mutex> lck(mRetryMutex);
                mRetryCV.notify_one();
            }*/
        }
        ~RemoteWriter() {
            setCloseflag(true);
            stopLogger();
            closeSocket();
        }

        COMMON_DEFINITIONS::eSTATUS connectToRemoteLogServer();
        COMMON_DEFINITIONS::eSTATUS sendMessage(const std::string &message);
        COMMON_DEFINITIONS::eSTATUS closeSocket();
        void processLogEvents();

    private:
        int mClientSocket = {-1};
        std::string mServerIPAddress;
        unsigned int mPortNumber;
        std::shared_ptr<std::thread> mRetryThread;
        std::condition_variable mRetryCV;
        std::mutex mRetryMutex;

        void retryConnection();
    };
}

#endif