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
            ~HTTPEventQueue() {
            }

            EVENT_MESSAGE::EventMessageInterface getEvent() override
            {
                Storage::SingleLLNode data = popEvent();
                EVENT_MESSAGE::HTTPEventMessage ret;
                ret.setMessage(data);
                ret.unpackMessage();

                return ret;
            }

            EVENT_MESSAGE::EventMessageInterface front() override
            {
                Storage::SingleLLNode data = frontEvent();
                EVENT_MESSAGE::HTTPEventMessage ret;
                ret.setMessage(data);
                ret.unpackMessage();

                return ret;
            }
    };
}

#endif