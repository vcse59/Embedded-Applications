#ifndef __DATABASE_CONNECTOR_H__
#define __DATABASE_CONNECTOR_H__

#include "CommonClasses/CommonDefinitions.h"
#include <unordered_set>

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
 

        private:
            DataBaseConnectorInterface(const DataBaseConnectorInterface&) = delete;
            DataBaseConnectorInterface& operator=(const DataBaseConnectorInterface&) = delete;
            DataBaseConnectorInterface(const DataBaseConnectorInterface&&) = delete;
            DataBaseConnectorInterface& operator=(const DataBaseConnectorInterface&&) = delete;
    };

    typedef std::shared_ptr<DATABASE_SERVICE::DataBaseConnectorInterface> S_PTR_DATABASE_CONNECTOR_INTERFACE;

}

#endif