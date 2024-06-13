#ifndef __HTTP_EVENT_QUEUE_H__
#define __HTTP_EVENT_QUEUE_H__

#include "Interfaces/EventQueueInterface.h"
#include "Modules/EventMessage/HTTPEventMessage.h"

namespace EVENT_MESSAGE
{
    class HTTPEventQueue : public EVENT_MESSAGE::EventQueueInterface
    {
        public:
            HTTPEventQueue() {}
            ~HTTPEventQueue() {}

            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> getEvent() override
            {
                std::shared_ptr<Storage::SingleLLNode> data = popEvent();
                std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> ret = std::make_shared<EVENT_MESSAGE::HTTPEventMessage>();
                ret->setMessage(*data);
                ret->unpackMessage();

                return ret;
            }

            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> front() override
            {
                std::shared_ptr<Storage::SingleLLNode> data = frontEvent();
                std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> ret = std::make_shared<EVENT_MESSAGE::HTTPEventMessage>();
                ret->setMessage(*data);
                ret->unpackMessage();

                return ret;
            }
    };
}

#endif