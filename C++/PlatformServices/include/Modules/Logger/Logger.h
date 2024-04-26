//
//  CLogger.h
//  ThreadProgramming
//
//  Created by Vivek Yadav on 11/11/19.
//  Copyright © 2019 Vivek Yadav. All rights reserved.
//

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <string>
#include <iostream>
#include <mutex>
#include "CommonClasses/CommonDefinitions.h"
#include "Modules/Logger/LogLevel.h"

namespace LOGGER_SERVICE
{
    class Logger
    {
        public:
            Logger(LOGGER_SERVICE::eLOG_LEVEL_ENUM    pAppLogLevel = LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG);
            ~Logger();

            std::ostream& operator<<(const char *pLogMessageStream);
            std::ostream& operator<<(std::string pLogMessageStream);
            std::ostream& operator<<(unsigned int pLogMessageStream);
            Logger& operator()(LOGGER_SERVICE::eLOG_LEVEL_ENUM pLogLevel = LOGGER_SERVICE::eLOG_LEVEL_ENUM::INFO_LOG);

        protected:
            LOGGER_SERVICE::eLOG_LEVEL_ENUM  mDefaultLogLevel;
            std::string getCurrentDateTime();
            LOGGER_SERVICE::LogLevel	mLogLevel;
            std::mutex                          lMutex;
            LOGGER_SERVICE::eLOG_LEVEL_ENUM  mAppLogLevel;
    };

    typedef std::shared_ptr<LOGGER_SERVICE::Logger> S_PTR_LOGGER;
}
#endif /* CLogger_h */