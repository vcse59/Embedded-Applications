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
        LogWriterInterface();
        virtual ~LogWriterInterface();
        virtual void write(const std::string logMessage);
        void set(std::ostream *os) { outObject = os; }
        void processLogMessage();

    protected:
        std::shared_ptr<std::thread> mLogThread = nullptr;
        std::ostream *outObject = nullptr;
        std::mutex mMutex;
        std::condition_variable mNotifyConsumer;
        std::condition_variable mNotifyProducer;
        bool readyToProcess = false;
    };

    typedef std::shared_ptr<LOGGER_SERVICE::LogWriterInterface> LOG_WRITER_SHARED_PTR;
}

#endif