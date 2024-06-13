#ifndef __DB_EVENT_QUEUE_H__
#define __DB_EVENT_QUEUE_H__

#include "Interfaces/EventQueueInterface.h"
#include "Modules/EventMessage/DBEventMessage.h"

namespace EVENT_MESSAGE
{
    class DBEventQueue : public EVENT_MESSAGE::EventQueueInterface
    {
        public:
            DBEventQueue() {}
            ~DBEventQueue() {}

            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> getEvent() override
            {
                std::shared_ptr<Storage::SingleLLNode> data = popEvent();
                std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> ret = std::make_shared<EVENT_MESSAGE::DBEventMessage>();
                ret->setMessage(*data);
                ret->unpackMessage();

                return ret;
            }
            
            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> front() override
            {
                std::shared_ptr<Storage::SingleLLNode> data = frontEvent();
                std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> ret = std::make_shared<EVENT_MESSAGE::DBEventMessage>();
                ret->setMessage(*data);
                ret->unpackMessage();

                return ret;
            }
    };
}

#endif