#ifndef __DBTABLE_INTERFACE_H__
#define __DBTABLE_INTERFACE_H__

#include <string>
#include <map>
#include "CommonClasses/CommonDefinitions.h"

namespace DATABASE_SERVICE
{
    class DataBaseTableInterface
    {
        public:
            DataBaseTableInterface(){}
            virtual ~DataBaseTableInterface() {}

            void setDataBaseName(std::string dbName) {mDBName = dbName;}
            void setTableName(std::string tableName) {mTableName = tableName;}
            virtual void addTableColumn(std::string fieldName, std::string fieldDataType) = 0;


            virtual COMMON_DEFINITIONS::eSTATUS setup() = 0;

        protected:
            std::string mDBName = {};
            std::string mTableName = {};
            std::map<std::string, std::string> mDbTableFieldDetails;
    };
    typedef std::shared_ptr<DATABASE_SERVICE::DataBaseTableInterface> S_PTR_DATABASE_TABLE_INTERFACE;
}

#endif