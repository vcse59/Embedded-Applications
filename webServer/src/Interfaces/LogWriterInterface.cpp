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
    outObject = nullptr;
}

void LogWriterInterface::write(const std::string logMessage)
{
    std::unique_lock<std::mutex> lck(mMutex);

    EVENT_MESSAGE::LoggerMessage message;
    strcpy(message.mLoggerString, logMessage.c_str());
    message.mMessageLen = logMessage.length();

    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE queueInterface = consoleApp->getLoggerQueueInterface();
    std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> event = std::make_shared<EVENT_MESSAGE::LoggerEventMessage>();
    event->setMessage(reinterpret_cast<char *>(&message), sizeof(message));
    queueInterface->pushEvent(event);

    mNotifyLoggerThread.notify_all(); // notify all waiting threads
}

void LogWriterInterface::processLogMessage()
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE queueInterface = consoleApp->getLoggerQueueInterface();

    while (true)
    {
        std::unique_lock<std::mutex> lck(mMutex);
        mNotifyLoggerThread.wait(lck, [&]
                                 { return (queueInterface->getQueueLength() > 0); });

        std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> elem1 = queueInterface->getEvent();
        EVENT_MESSAGE::LoggerMessage *message = (EVENT_MESSAGE::LoggerMessage *)elem1->getEventData();
        *outObject << message->mLoggerString;
        outObject->flush();
    }
}