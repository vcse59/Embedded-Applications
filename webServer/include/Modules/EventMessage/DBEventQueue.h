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
            COMMON_DEFINITIONS::eSTATUS initializeQueue() override
            {
                mEventQueue = std::make_shared<Storage::QueueContainer<Storage::SingleLLNode>>();
                return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
            }

            COMMON_DEFINITIONS::eSTATUS pushEvent(std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> msg) override
            {
                mEventQueue->enqueue(msg->getMessage());
                return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
            }

            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> popEvent() override
            {
                Storage::SingleLLNode data = mEventQueue->dequeue();
                std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> ret = std::make_shared<EVENT_MESSAGE::DBEventMessage>();
                ret->setMessage(data);
                ret->unpackMessage();
                return ret;
            }

            unsigned int getQueueLength() override {
                return mEventQueue->getLength();
            }
    };
}

#endif