/**
 * @file EventMessageInterface.h
 * @brief Header file for the EventMessageInterface class
 *
 * This file contains the declaration of the EventMessageInterface class,
 * which demonstrates interfaces to push/pop recieve events
 *
 * Contact: Vivek Yadav <v.cse59@gmail.com>
 *
 * @author Vivek Yadav
 * @date 2024-06-12
 */

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
    };

    class EventMessageInterface
    {
        public:
            EventMessageInterface() {}
            virtual ~EventMessageInterface() {}
            
            void packMessage(){
                mData.setData(reinterpret_cast<char*> (&eventMsg), sizeof (eventMsg));
            }
            void unpackMessage(){
                EventMessage* eventData = reinterpret_cast<EventMessage*>(mData.getData());
                memcpy(&eventMsg, eventData, sizeof(eventMsg));
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

            void setMessage(char *msg, unsigned int msgLen)
            {
                memset(eventMsg.mMessage, 0, sizeof(eventMsg.mMessage));
                memcpy(eventMsg.mMessage, msg, msgLen);
                eventMsg.mMessageLen = msgLen;
                packMessage();
            }

        protected:
            EventMessage eventMsg;
            Storage::SingleLLNode mData;
    };
}

#endif