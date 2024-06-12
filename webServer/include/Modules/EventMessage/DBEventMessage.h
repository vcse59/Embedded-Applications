#ifndef __DB_EVENT_MESSAGE_H__
#define __DB_EVENT_MESSAGE_H__

#include "Interfaces/EventMessageInterface.h"

namespace EVENT_MESSAGE
{

    class DBMessage{
        public:
            char mTableName[DB_TABLE_BUFFER_SIZE];
            char mQueryString[STRING_SIZE];
            COMMON_DEFINITIONS::eQUERY_TYPE mQueryType;
    };
    class DBEventMessage : public EVENT_MESSAGE::EventMessageInterface
    {
    public:
        DBEventMessage(){}
        ~DBEventMessage(){}
    };
}

#endif