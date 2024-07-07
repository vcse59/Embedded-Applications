#include "Interfaces/LogWriterInterface.h"
#include "Modules/ConsoleMain.h"

using namespace LOGGER_SERVICE;

LogWriterInterface::LogWriterInterface(std::ostream *os) : outObject(os)
{
    mLogThread = std::make_shared<std::thread>(&LOGGER_SERVICE::LogWriterInterface::processLogMessage, this);
    mLogThread->detach();
}

LogWriterInterface::LogWriterInterface()
{
    mLogThread = std::make_shared<std::thread>(&LOGGER_SERVICE::LogWriterInterface::processLogMessage, this);
    mLogThread->detach();
}

LogWriterInterface::LogWriterInterface(std::function<void()> funcCallback)
{
    mLogThread = std::make_shared<std::thread>(funcCallback);
    mLogThread->detach();
}

LogWriterInterface::~LogWriterInterface()
{
    setCloseflag(true);
    stopLogger();
    outObject = nullptr;
}

void LogWriterInterface::write(const std::string logMessage)
{
    std::unique_lock<std::mutex> lck(mMutex);

    EVENT_MESSAGE::LoggerMessage message;
    strcpy(message.mLoggerString, logMessage.c_str());
    message.mMessageLen = logMessage.length();

    std::weak_ptr<FRAMEWORK::ConsoleAppInterface> weakPtr(FRAMEWORK::ConsoleMain::getConsoleAppInterface());
    std::weak_ptr<EVENT_MESSAGE::EventQueueInterface> eventWeakPtr(weakPtr.lock()->getLoggerQueueInterface());

    auto queueInterface = eventWeakPtr.lock();
    EVENT_MESSAGE::LoggerEventMessage event;
    event.setMessage(reinterpret_cast<char *>(&message), sizeof(message));
    queueInterface->pushEvent(event);
    
    mNotifyLoggerThread.notify_one(); // notify all waiting threads
}

void LogWriterInterface::processLogMessage()
{
    std::weak_ptr<FRAMEWORK::ConsoleAppInterface> weakPtr(FRAMEWORK::ConsoleMain::getConsoleAppInterface());
    std::weak_ptr<EVENT_MESSAGE::EventQueueInterface> eventWeakPtr(weakPtr.lock()->getLoggerQueueInterface());

    auto queueInterface = eventWeakPtr.lock();
    while (true)
    {
        std::unique_lock<std::mutex> lck(mMutex);
        mNotifyLoggerThread.wait(lck);
        if (mNeedToClose)
        {
            std::cout << "Shutting down the logger thread" << std::endl;
            break;
        }

        if (queueInterface->getQueueLength() > 0){
            EVENT_MESSAGE::EventMessageInterface elem1 = queueInterface->getEvent();
            EVENT_MESSAGE::LoggerMessage *message = (EVENT_MESSAGE::LoggerMessage *)elem1.getEventData();
            *outObject << message->mLoggerString;
            outObject->flush();
        }
    }
}