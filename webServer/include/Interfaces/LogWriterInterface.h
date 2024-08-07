#ifndef __LOG_WRITER_INTERFACE_H__
#define __LOG_WRITER_INTERFACE_H__

#include <mutex>
#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <condition_variable>

#include "Modules/EventMessage/LoggerEventMessage.h"
#include "Modules/EventMessage/LoggerEventQueue.h"

namespace LOGGER_SERVICE{

    class LogWriterInterface
    {
    public:
        LogWriterInterface(std::ostream *os);
        LogWriterInterface(std::function<void()> funcCallback);
        LogWriterInterface();
        virtual ~LogWriterInterface();
        virtual void write(const std::string logMessage);
        void set(std::ostream *os) { outObject = os; }
        void stopLogger(){
            std::unique_lock<std::mutex> lck(mMutex);
            mNotifyLoggerThread.notify_one();
        }
        
        void setCloseflag(bool closeFlag)
        {
            std::unique_lock<std::mutex> lck(mMutex);
            mNeedToClose = closeFlag;
        }

    protected:
        bool mNeedToClose = false;
        std::shared_ptr<std::thread> mLogThread = nullptr;
        std::ostream *outObject = nullptr;
        std::mutex mMutex;
        std::condition_variable mNotifyLoggerThread;
        virtual void processLogMessage();
    };

    typedef std::shared_ptr<LOGGER_SERVICE::LogWriterInterface> LOG_WRITER_SHARED_PTR;
}

#endif