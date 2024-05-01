#ifndef __DATABASE_CONNECTOR_H__
#define __DATABASE_CONNECTOR_H__

#include "CommonClasses/CommonDefinitions.h"

namespace DATABASE_SERVICE
{
    class DataBaseConnectorInterface
    {
        public:
            DataBaseConnectorInterface(){}
            virtual ~DataBaseConnectorInterface() {}

            virtual COMMON_DEFINITIONS::eSTATUS connectToDBService() = 0;
            virtual COMMON_DEFINITIONS::eSTATUS createDataBase(std::string dbName) = 0;
            virtual COMMON_DEFINITIONS::eSTATUS createTable(std::string tableName) = 0;
            virtual COMMON_DEFINITIONS::eSTATUS executeQuery(std::string tableName, 
                                                        std::string queryString,
                                                        COMMON_DEFINITIONS::eQUERY_TYPE queryType) = 0;

        private:
            DataBaseConnectorInterface(const DataBaseConnectorInterface&) = delete;
            DataBaseConnectorInterface& operator=(const DataBaseConnectorInterface&) = delete;
            DataBaseConnectorInterface(const DataBaseConnectorInterface&&) = delete;
            DataBaseConnectorInterface& operator=(const DataBaseConnectorInterface&&) = delete;
    };

    typedef std::shared_ptr<DATABASE_SERVICE::DataBaseConnectorInterface> S_PTR_DATABASE_CONNECTOR_INTERFACE;

}

#endif