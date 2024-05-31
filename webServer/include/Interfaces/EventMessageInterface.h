#ifndef __EVENT_MESSAGE_INTERFACE_H__
#define __EVENT_MESSAGE_INTERFACE_H__

#include "CommonClasses/CommonDefinitions.h"
#include "Containers/LinkedList.h"

namespace EVENT_MESSAGE
{
    class EventMessage{
        public:
            char mMessage[MAX_BUFFER_SIZE] = {'\0',};
            unsigned int mMessageLen = 0;
            COMMON_DEFINITIONS::MESSAGE_TYPE mMessageType = COMMON_DEFINITIONS::MESSAGE_TYPE::UNKNOWN_MESSAGE;
    };

    class EventMessageInterface
    {
        public:
            EventMessageInterface() {}
            virtual ~EventMessageInterface() {}

            virtual void setMessage(char* msg, unsigned int msgLen, COMMON_DEFINITIONS::MESSAGE_TYPE messageType) = 0;
            
            void packMessage(){
                mData.setData(reinterpret_cast<char*> (&eventMsg), sizeof (eventMsg));
            }
            void unpackMessage(){
                EventMessage* eventData = reinterpret_cast<EventMessage*>(mData.getData());
                memcpy(&eventMsg, eventData, sizeof(eventMsg));
            }

            COMMON_DEFINITIONS::MESSAGE_TYPE getMessageType() const{
                return eventMsg.mMessageType;
            }

            char* getEventData(){
                return eventMsg.mMessage;
            }
            
            unsigned int getMsgLen() const
            {
                return eventMsg.mMessageLen;
            }

            Storage::SingleLLNode getMessage() {
                return mData;
            };

            void setMessage(Storage::SingleLLNode node){
                mData = node;
            }

            

        protected:
            EventMessage eventMsg;
            Storage::SingleLLNode mData;
    };
}

#endif