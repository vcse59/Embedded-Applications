#ifndef __DATABASE_CONNECTOR_H__
#define __DATABASE_CONNECTOR_H__

#include "CommonClasses/CommonDefinitions.h"
#include <unordered_set>
#include <mutex>
#include <condition_variable>

namespace DATABASE_SERVICE
{
    class DataBaseConnectorInterface
    {
        public:
            DataBaseConnectorInterface(){}
            virtual ~DataBaseConnectorInterface() {}

            virtual COMMON_DEFINITIONS::eSTATUS initializeDB() = 0;
            virtual COMMON_DEFINITIONS::eSTATUS executeQuery(std::string tableName, 
                                                        std::string queryString,
                                                        COMMON_DEFINITIONS::eQUERY_TYPE queryType) = 0;
            virtual COMMON_DEFINITIONS::eSTATUS loadAccessToken(std::unordered_set<std::string>& accessToken) = 0;
 
            void notifyDBThread(){
                std::unique_lock<std::mutex> lck(mMutex);
                mNotifyConsumer.notify_one(); // notify all waiting threads
                readyToProcess = true;
                while (readyToProcess)
                    mNotifyProducer.wait(lck);
            }

        protected:
            std::mutex mMutex;
            std::condition_variable mNotifyConsumer;
            std::condition_variable mNotifyProducer;
            bool readyToProcess = false;

        private:
            DataBaseConnectorInterface(const DataBaseConnectorInterface &) = delete;
            DataBaseConnectorInterface& operator=(const DataBaseConnectorInterface&) = delete;
            DataBaseConnectorInterface(const DataBaseConnectorInterface&&) = delete;
            DataBaseConnectorInterface& operator=(const DataBaseConnectorInterface&&) = delete;
    };

    typedef std::shared_ptr<DATABASE_SERVICE::DataBaseConnectorInterface> S_PTR_DATABASE_CONNECTOR_INTERFACE;

}

#endif