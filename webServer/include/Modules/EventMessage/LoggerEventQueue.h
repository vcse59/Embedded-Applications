#ifndef __LOGGER_EVENT_QUEUE_H__
#define __LOGGER_EVENT_QUEUE_H__

#include "Interfaces/EventQueueInterface.h"
#include "Modules/EventMessage/LoggerEventMessage.h"

namespace EVENT_MESSAGE
{
    class LoggerEventQueue : public EVENT_MESSAGE::EventQueueInterface
    {
        public:
            LoggerEventQueue() {}
            ~LoggerEventQueue() {
            }

            EVENT_MESSAGE::EventMessageInterface getEvent() override
            {
                Storage::SingleLLNode data = popEvent();
                EVENT_MESSAGE::LoggerEventMessage ret;
                ret.setMessage(data);
                ret.unpackMessage();
                return ret;
            }

            EVENT_MESSAGE::EventMessageInterface front() override
            {
                Storage::SingleLLNode data = frontEvent();
                EVENT_MESSAGE::LoggerEventMessage ret;
                ret.setMessage(data);
                ret.unpackMessage();

                return ret;
            }
    };
}

#endif