//
//  CLogger.cpp
//  ThreadProgramming
//
//  Created by Vivek Yadav on 11/11/19.
//  Copyright © 2019 Vivek Yadav. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "Modules/Logger/Logger.h"
#include "Modules/Logger/LogLevel.h"

using namespace LOGGER_SERVICE;

Logger::Logger(eLOG_LEVEL_ENUM    pAppLogLevel)
	:mAppLogLevel(pAppLogLevel)
{
    this->mDefaultLogLevel  =   eLOG_LEVEL_ENUM::DEBUG_LOG;
}

Logger::~Logger()
{
}

std::string Logger::getCurrentDateTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[128];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%Y-%d-%m %H:%M:%S",timeinfo);
    std::string str(buffer);
    return buffer;
}

std::ostream& Logger::operator<<(const char *pLogMessageStream)
{
    std::string lLogLevelStr = mLogLevel.getLogLevelString(this->mDefaultLogLevel);
    lMutex.lock();
    std::ostream& lOStream = std::cout << this->getCurrentDateTime() << " " << lLogLevelStr << " " << pLogMessageStream;
    lMutex.unlock();
    return lOStream;
}

std::ostream& Logger::operator<<(unsigned int pLogMessageStream)
{
    std::string lLogLevelStr = mLogLevel.getLogLevelString(this->mDefaultLogLevel);
    lMutex.lock();
    std::ostream& lOStream = std::cout << this->getCurrentDateTime() << " " << lLogLevelStr << " " << pLogMessageStream;
    lMutex.unlock();
    return lOStream;
}

std::ostream& Logger::operator<<(std::string pLogMessageStream)
{
    std::string lLogLevelStr = mLogLevel.getLogLevelString(this->mDefaultLogLevel);
    lMutex.lock();
    std::ostream& lOStream = std::cout << this->getCurrentDateTime() << " " << lLogLevelStr << " " << pLogMessageStream;
    lMutex.unlock();
    return lOStream;
}

Logger& Logger::operator()(eLOG_LEVEL_ENUM pLogLevel)
{
    std::lock_guard<std::mutex> lock(lMutex);
    this->mDefaultLogLevel = pLogLevel;
    return *this;
}