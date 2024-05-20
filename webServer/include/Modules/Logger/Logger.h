//
//  CLogger.h
//  ThreadProgramming
//
//  Created by Vivek Yadav on 11/11/19.
//  Copyright © 2019 Vivek Yadav. All rights reserved.
//

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <mutex>
#include <streambuf>
#include <string>
#include <sstream>
#include "CommonClasses/CommonDefinitions.h"
#include "Modules/Logger/LogLevel.h"

#define LOGGER(x)   *x

namespace LOGGER_SERVICE
{
    class CustomLogBuf: public std::streambuf{

        public:
            CustomLogBuf(LOGGER_SERVICE::eLOG_LEVEL_ENUM    pAppLogLevel) : mAppLogLevel(pAppLogLevel){}

        protected:
            int sync() override {
                if (!buffer_.empty() && buffer_.back() != '\n') {
                    buffer_ += '\n';
                }
                if (!buffer_.empty()) {
                    std::cout << getCurrentDateTime() << " " << mLogLevel.getLogLevelString(mAppLogLevel) << " " << buffer_;
                    std::cout.flush();
                    buffer_.clear();
                }
                return 0;
            }

            int overflow(int c) override {
                if (c != EOF) {
                    buffer_ += static_cast<char>(c);
                }
                return c;
            }

        std::string getCurrentDateTime()
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
        
        protected:
            LOGGER_SERVICE::LogLevel	mLogLevel;
            std::mutex                          lMutex;
            LOGGER_SERVICE::eLOG_LEVEL_ENUM  mAppLogLevel = LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG;

        private:
            std::string buffer_;
    };

    class LoggerStream : public std::ostream
    {
        public:
            LoggerStream(LOGGER_SERVICE::eLOG_LEVEL_ENUM    pAppLogLevel = LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG);
            ~LoggerStream();

            LoggerStream& operator*(){
                return *this;
            }

        private:
            LoggerStream(const LoggerStream&) = delete;
            LoggerStream& operator=(const LoggerStream&) = delete;
            LoggerStream(const LoggerStream&&) = delete;
            LoggerStream& operator=(const LoggerStream&&) = delete;
            LOGGER_SERVICE::CustomLogBuf buffer_;
    };

    typedef std::shared_ptr<LOGGER_SERVICE::LoggerStream> S_PTR_LOGGER;
}
#endif /* CLogger_h */