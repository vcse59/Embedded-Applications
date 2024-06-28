#ifndef __LOGGER_EVENT_MESSAGE_H__
#define __LOGGER_EVENT_MESSAGE_H__

#include "Interfaces/EventMessageInterface.h"

namespace EVENT_MESSAGE
{

    class LoggerMessage{
        public:
            char mLoggerString[STRING_SIZE];
            unsigned int mMessageLen = 0;
    };
    class LoggerEventMessage : public EVENT_MESSAGE::EventMessageInterface
    {
    public:
        LoggerEventMessage() {}
        ~LoggerEventMessage() {
        }
    };
}

#endif