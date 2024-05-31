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
            virtual COMMON_DEFINITIONS::eSTATUS initializeQueue() = 0;
            virtual COMMON_DEFINITIONS::eSTATUS pushEvent(std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> msg) = 0;
            virtual std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> popEvent() = 0;
            virtual unsigned int getQueueLength() = 0;
        protected:
            std::shared_ptr<Storage::QueueContainer<Storage::SingleLLNode>> mEventQueue;
    };

    typedef std::shared_ptr<EVENT_MESSAGE::EventQueueInterface> S_PTR_EVENT_QUEUE_INTERFACE;
}

#endif