#ifndef __DATABASE_TABLE_H__
#define __DATABASE_TABLE_H__

#include "Interfaces/DataBaseTableInterface.h"
#include "Modules/Logger/Logger.h"

namespace DATABASE_SERVICE
{
    class DataBaseTable : public DATABASE_SERVICE::DataBaseTableInterface
    {
        public:
            DataBaseTable(LOGGER_SERVICE::S_PTR_LOGGER logger);
            ~DataBaseTable();

            void addTableColumn(std::string fieldName, std::string fieldDataType) override;
            COMMON_DEFINITIONS::eSTATUS setup() override;
        private:
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            DataBaseTable(const DataBaseTable&) = delete;
            DataBaseTable& operator=(const DataBaseTable&) = delete;
            DataBaseTable(const DataBaseTable&&) = delete;
            DataBaseTable& operator=(const DataBaseTable&&) = delete;
    };
}

#endif