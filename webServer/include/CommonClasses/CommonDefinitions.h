/**
 * @file CommonDefintions.h
 *
 * @brief This file declares all enumeration and common defintions
 *
 * @author Vivek Yadav
 * Contact: v.cse59@gmail.com
 *
 */

#ifndef __COMMON_DEFINITIONS_H__
#define __COMMON_DEFINITIONS_H__

//Include system header files
#include <cstddef>
#include <iostream>
#include <chrono>
#include <cstring>
#include <memory>

// Pre-Processor Directive
#define MAX_CONNECTIONS 64 
#define MAX_BUFFER_SIZE 1024
#define PORT 9999 
#define MAX_HEADER_SIZE 2048
#define MAX_BODY_SIZE   8192
#define SERVER_LISTENDER_TIMEOUT_IN_MS 2

namespace COMMON_DEFINITIONS
{
    // Status Enumeration
    enum eSTATUS
    {
        UNKNOWN                         = 0,
        SOCKET_CREATED                  = 1,
        SOCKET_CONNECTED                = 2,
        SOCKET_INITIALIZATION_FAILED    = 3,
        SUCCESS                         = 4,
        ERROR                           = 5,
        NOT_FOUND                       = 6,
        EXISTS                          = 7,
        SOCKET_CONNECTION_IN_PROGRESS   = 8
    };

    enum eQUERY_TYPE
    {
        UNKNOWN_QUERY       = 0,
        DATA_QUERY          = 1,
        DATA_MANIPULATION   = 2,
        DATA_DEFINITION     = 3
    };
    
    enum eHTTP_SESSION_STATUS
    {
        UNKNOWN_SESSION     = 0,
        SESSION_ADDED       = 1,
        SESSION_EXISTS      = 2,
        SESSION_EXPIRED     = 3,
        SESSION_DELETED     = 4,
        SESSION_VALID       = 5
    };

    // Class to keep generic data
    class SingleLLNode
    {
        public:
            // Parameterized Constructor
            SingleLLNode(char *data, unsigned int dataLength) {
                mData = new char[dataLength];
                setData(data, dataLength);
                }
            // Default Constructor
            SingleLLNode() {}

            // Destructor
            ~SingleLLNode() {
                if (mData != NULL)
                {
                    delete [] mData;
                }

                mData = NULL;
                mKey  = 0;
                mDataLength = 0;
                mNext = NULL;
            }

            // Copy Constructor
            SingleLLNode(const SingleLLNode& instance)
            {
                mKey = instance.getKey();
                mNext = NULL;
                mDataLength = instance.getDataLength();
                mData = new char[mDataLength];
                memcpy(mData, instance.getData(), mDataLength);
            }

            // Assignment operator
            SingleLLNode& operator=(const SingleLLNode& instance)
            {
                if (mData != NULL)
                {
                    delete [] mData;
                    mData = NULL;
                    mDataLength = 0;
                    mKey = 0;
                    mNext = NULL;
                }

                mKey = instance.getKey();
                mNext = NULL;
                mDataLength = instance.getDataLength();
                mData = new char[mDataLength];
                memcpy(mData, instance.getData(), mDataLength);

                return *this;
            }

            // Setting next node memory address
            void setNext(SingleLLNode *node) { mNext = node; }

            // Retrieving next node memory address
            SingleLLNode *getNext() { return mNext; }

            // Set data
            void setData(char *data, unsigned int dataLength)
            {
                std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
                std::chrono::system_clock::duration dtn = tp.time_since_epoch();
                mKey = dtn.count();
                memcpy(mData, data, dataLength);
                mDataLength = dataLength;
            }

            // Get Data
            char* getData() const {return mData;}

            // Get Key
            unsigned long getKey() const {return mKey;}

            // Get Data Length
            unsigned int getDataLength() const {return mDataLength;}

        private:

            char* mData = NULL;
            unsigned int mDataLength = {0};
            unsigned long mKey = {0};
            COMMON_DEFINITIONS::SingleLLNode* mNext = NULL;
            SingleLLNode(const SingleLLNode&&) = delete;
            SingleLLNode& operator=(const SingleLLNode&&) = delete;
    };
}
#endif