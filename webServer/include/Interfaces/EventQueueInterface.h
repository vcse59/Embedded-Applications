#ifndef __EVENT_QUEUE_INTERFACE_H__
#define __EVENT_QUEUE_INTERFACE_H__

#include "CommonClasses/CommonDefinitions.h"
#include "Containers/Queue.h"
#include "Interfaces/EventMessageInterface.h"

namespace EVENT_MESSAGE
{
    class EventQueueInterface
    {
        public:
            EventQueueInterface(){}
            virtual ~EventQueueInterface() {}

            COMMON_DEFINITIONS::eSTATUS initializeQueue()
            {
                mEventQueue = std::make_shared<Storage::QueueContainer<Storage::SingleLLNode>>();
                return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
            }

            unsigned int getQueueLength()
            {
                return mEventQueue->getLength();
            }

            virtual std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> getEvent() = 0;

            COMMON_DEFINITIONS::eSTATUS pushEvent(std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> msg)
            {
                mEventQueue->enqueue(msg->getMessage());
                return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
            }

            virtual std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> front() = 0;

        protected:
            std::shared_ptr<Storage::SingleLLNode> popEvent()
            {
                return std::make_shared<Storage::SingleLLNode>(mEventQueue->dequeue());
            }

            std::shared_ptr<Storage::SingleLLNode> frontEvent()
            {
                return std::make_shared<Storage::SingleLLNode>(mEventQueue->front());
            }

            std::shared_ptr<Storage::QueueContainer<Storage::SingleLLNode>> mEventQueue;
    };

    typedef std::shared_ptr<EVENT_MESSAGE::EventQueueInterface> S_PTR_EVENT_QUEUE_INTERFACE;
}

#endif