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
            virtual ~EventQueueInterface(){}
            unsigned int getQueueLength()
            {
                return mEventQueue.getLength();
            }

            virtual EVENT_MESSAGE::EventMessageInterface getEvent() = 0;

            COMMON_DEFINITIONS::eSTATUS pushEvent(EVENT_MESSAGE::EventMessageInterface& msg)
            {
                mEventQueue.enqueue(msg.getMessage());
                return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
            }

            virtual EVENT_MESSAGE::EventMessageInterface front() = 0;

        protected:
            Storage::SingleLLNode popEvent()
            {
                return mEventQueue.dequeue();
            }

            Storage::SingleLLNode frontEvent()
            {
                return mEventQueue.front();
            }

            Storage::QueueContainer<Storage::SingleLLNode> mEventQueue;
    };

    typedef std::shared_ptr<EVENT_MESSAGE::EventQueueInterface> S_PTR_EVENT_QUEUE_INTERFACE;
}

#endif