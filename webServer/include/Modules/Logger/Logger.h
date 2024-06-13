#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <mutex>
#include <streambuf>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "Modules/Logger/LogLevel.h"
#include "Interfaces/LogWriterInterface.h"

namespace LOGGER_SERVICE
{
    class LogWriterInterface;
    #define LOGGER(x)   *x

    class CustomLogBuf : public std::streambuf
    {
    public:
        CustomLogBuf(LOGGER_SERVICE::LOG_WRITER_SHARED_PTR logWriteSharedPtr,
                    LOGGER_SERVICE::eLOG_LEVEL_ENUM pAppLogLevel)
            : mAppLogLevel(pAppLogLevel), m_LogWriter(logWriteSharedPtr)
        {
        }

        ~CustomLogBuf()
        {
            m_LogWriter = nullptr;
        }

    protected:
        int sync() override
        {
            std::lock_guard<std::mutex> lock(lMutex);
            if (!buffer_.empty() && buffer_.back() != '\n')
            {
                buffer_ += '\n';
            }
            if (!buffer_.empty())
            {
                std::stringstream ss;
                ss << getCurrentDateTime() << " " << mLogLevel.getLogLevelString(mAppLogLevel) << " " << buffer_;
                m_LogWriter->write(ss.str());
                buffer_.clear();
            }
            return 0;
        }

        int overflow(int c) override
        {
            std::lock_guard<std::mutex> lock(lMutex);
            if (c != EOF)
            {
                buffer_ += static_cast<char>(c);
            }
            return c;
        }

        std::string getCurrentDateTime()
        {
            // Get the current time point from system clock
            auto now = std::chrono::system_clock::now();

            // Convert to time_t which is used by C library to represent time
            std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

            // Convert to tm struct for local time
            std::tm now_tm = *std::localtime(&now_time_t);

            // Extract milliseconds from the time_point
            auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

            // Create a string stream to format the output
            std::ostringstream oss;
            oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << now_ms.count();

            return oss.str();
        }

    protected:
        LOGGER_SERVICE::LOG_WRITER_SHARED_PTR m_LogWriter = nullptr;
        LOGGER_SERVICE::LogLevel mLogLevel;
        std::mutex lMutex;
        LOGGER_SERVICE::eLOG_LEVEL_ENUM mAppLogLevel = LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG;

    private:
        std::string buffer_;
    };

    class LoggerStream : public std::ostream
    {
    public:
        LoggerStream(LOGGER_SERVICE::LOG_WRITER_SHARED_PTR logPtr,
                     LOGGER_SERVICE::eLOG_LEVEL_ENUM pAppLogLevel = LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG,
                     LOGGER_SERVICE::LOG_OUTPUT loggerOutputEnum = LOGGER_SERVICE::LOG_OUTPUT::LOG_OUTPUT_CONSOLE)
            : std::ostream(&buffer_), buffer_(logPtr, pAppLogLevel),mLoggerOutputEnum(loggerOutputEnum)
        {
        }
        ~LoggerStream() {
            flush();
        }

        LoggerStream &operator*()
        {
            return *this;
        }

    private:
        LOGGER_SERVICE::CustomLogBuf buffer_;
        LOGGER_SERVICE::LOG_OUTPUT mLoggerOutputEnum = LOGGER_SERVICE::LOG_OUTPUT::LOG_OUTPUT_CONSOLE;
    };
    typedef std::shared_ptr<LOGGER_SERVICE::LoggerStream> S_PTR_LOGGER;
}

#endif