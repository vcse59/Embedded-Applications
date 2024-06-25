/**
 * @file DataBaseConnectorInterface.h
 * @brief Header file for the DataBaseConnectorInterface class
 *
 * This file contains the declaration of the DataBaseConnectorInterface class,
 * which demonstrates interfaces to communicate with DB
 *
 * Contact: Vivek Yadav <v.cse59@gmail.com>
 *
 * @author Vivek Yadav
 * @date 2024-06-12
 */

#ifndef __DATABASE_CONNECTOR_H__
#define __DATABASE_CONNECTOR_H__

#include "CommonClasses/CommonDefinitions.h"
#include <unordered_set>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace DATABASE_SERVICE
{
    class DataBaseConnectorInterface
    {
    public:
        DataBaseConnectorInterface() {}
        virtual ~DataBaseConnectorInterface() {}

        virtual COMMON_DEFINITIONS::eSTATUS initializeDB() = 0;
        virtual COMMON_DEFINITIONS::eSTATUS executeQuery(std::string tableName,
                                                         std::string queryString,
                                                         COMMON_DEFINITIONS::eQUERY_TYPE queryType) = 0;
        virtual COMMON_DEFINITIONS::eSTATUS loadAccessToken(std::unordered_set<std::string> &accessToken) = 0;

        void notifyDBThread()
        {
            std::unique_lock<std::mutex> lck(mMutex);
            mNotifyDBThread.notify_one(); // notify all waiting threads
        }

        void notifyCommitThread()
        {
            std::unique_lock<std::mutex> Commitlck(mCommitMutex);
            mNotifyCommitThread.notify_one(); // notify waiting commit thread
        }

        unsigned int getCurrentRecordCount() const{
            return mRecordCount;
        }

    protected:
        std::mutex mMutex;
        std::mutex mCommitMutex;
        std::condition_variable mNotifyDBThread;
        std::condition_variable mNotifyCommitThread;
        std::shared_ptr<std::thread> m_DBThread = nullptr;
        std::shared_ptr<std::thread> m_CommitThread = nullptr;
        unsigned int mRecordCount = 0;
        virtual void processDBEvents() = 0;
        virtual void commitDBTransaction() = 0;

    private:
        DataBaseConnectorInterface(const DataBaseConnectorInterface &) = delete;
        DataBaseConnectorInterface &operator=(const DataBaseConnectorInterface &) = delete;
        DataBaseConnectorInterface(const DataBaseConnectorInterface &&) = delete;
        DataBaseConnectorInterface &operator=(const DataBaseConnectorInterface &&) = delete;
    };

    typedef std::shared_ptr<DATABASE_SERVICE::DataBaseConnectorInterface> S_PTR_DATABASE_CONNECTOR_INTERFACE;

}

#endif