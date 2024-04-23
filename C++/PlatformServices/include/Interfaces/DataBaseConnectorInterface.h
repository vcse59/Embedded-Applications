#ifndef __DATABASE_CONNECTOR_H__
#define __DATABASE_CONNECTOR_H__

#include "CommonClasses/CommonDefinitions.h"

namespace DATABASE_SERVICE
{
    class DataBaseConnectorInterface
    {
    public:
        virtual ~DataBaseConnectorInterface() {}

        virtual COMMON_DEFINITIONS::eSTATUS connectToDBService() = 0;
        virtual COMMON_DEFINITIONS::eSTATUS createDataBase(std::string dbName) = 0;
        virtual COMMON_DEFINITIONS::eSTATUS createTable(std::string tableName) = 0;
        virtual COMMON_DEFINITIONS::eSTATUS executeQuery(std::string tableName, 
                                                        std::string queryString,
                                                        COMMON_DEFINITIONS::eQUERY_TYPE queryType) = 0;

        // COMMON_DEFINITIONS::eSTATUS dropTable(std::string tableName);

        // COMMON_DEFINITIONS::eSTATUS switchDatabase(std::string dbName);

        // COMMON_DEFINITIONS::eSTATUS insertRecord();
        // COMMON_DEFINITIONS::eSTATUS DeleteRecord();
        // COMMON_DEFINITIONS::eSTATUS UpdateRecord();
        //  TODO - Fetch record
    };

    typedef std::shared_ptr<DATABASE_SERVICE::DataBaseConnectorInterface> S_PTR_DATABASE_CONNECTOR_INTERFACE;

}

#endif