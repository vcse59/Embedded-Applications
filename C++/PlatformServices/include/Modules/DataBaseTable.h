#ifndef __PET_TABLE_H__
#define __PET_TABLE_H__

#include "Interfaces/DataBaseTableInterface.h"

namespace DATABASE_SERVICE
{
    class DataBaseTable : public DATABASE_SERVICE::DataBaseTableInterface
    {
        public:
            DataBaseTable();
            ~DataBaseTable();

            void addTableColumn(std::string fieldName, std::string fieldDataType) override;
            COMMON_DEFINITIONS::eSTATUS setup() override;
    };
}

#endif