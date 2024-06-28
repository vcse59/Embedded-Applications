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
            ~DBEventQueue() {
            }

            EVENT_MESSAGE::EventMessageInterface getEvent() override
            {
                Storage::SingleLLNode data = popEvent();
                EVENT_MESSAGE::DBEventMessage ret;
                ret.setMessage(data);
                ret.unpackMessage();

                return ret;
            }
            
            EVENT_MESSAGE::EventMessageInterface front() override
            {
                Storage::SingleLLNode data = frontEvent();
                EVENT_MESSAGE::DBEventMessage ret;
                ret.setMessage(data);
                ret.unpackMessage();

                return ret;
            }
    };
}

#endif