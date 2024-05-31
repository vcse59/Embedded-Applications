#ifndef __DB_EVENT_MESSAGE_H__
#define __DB_EVENT_MESSAGE_H__

#include "Interfaces/EventMessageInterface.h"

namespace EVENT_MESSAGE
{

    class DBMessage{
        public:
            char mTableName[DB_TABLE_BUFFER_SIZE];
            char mQueryString[DB_QUERY_STRING_SIZE];
            COMMON_DEFINITIONS::eQUERY_TYPE mQueryType;
    };
    class DBEventMessage : public EVENT_MESSAGE::EventMessageInterface
    {
    public:
        DBEventMessage(){}
        ~DBEventMessage(){}

        void setMessage(char *msg, unsigned int msgLen, COMMON_DEFINITIONS::MESSAGE_TYPE messageType) override
        {
            memset(eventMsg.mMessage, 0, sizeof(eventMsg.mMessage));
            memcpy(eventMsg.mMessage, msg, msgLen);
            eventMsg.mMessageLen = msgLen;
            eventMsg.mMessageType = messageType;
            packMessage();
        }
    };
}

#endif