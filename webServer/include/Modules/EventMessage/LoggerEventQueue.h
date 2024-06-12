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
            ~LoggerEventQueue() {}

            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> getEvent() override
            {
                std::shared_ptr<Storage::SingleLLNode> data = popEvent();
                std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> ret = std::make_shared<EVENT_MESSAGE::LoggerEventMessage>();
                ret->setMessage(*data);
                ret->unpackMessage();

                return ret;
            }
    };
}

#endif