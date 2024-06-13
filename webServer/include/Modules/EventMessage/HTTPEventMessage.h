#ifndef __HTTP_EVENT_MESSAGE_H__
#define __HTTP_EVENT_MESSAGE_H__

#include "Interfaces/EventMessageInterface.h"

namespace EVENT_MESSAGE
{

    class HTTPMessage{
        public:
            int socketId;
    };
    class HTTPEventMessage : public EVENT_MESSAGE::EventMessageInterface
    {
    public:
        HTTPEventMessage() {}
        ~HTTPEventMessage() {}
    };
}

#endif