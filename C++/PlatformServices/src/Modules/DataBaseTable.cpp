#include "Modules/DataBaseTable.h"
#include <iterator>

using namespace DATABASE_SERVICE;

DataBaseTable::DataBaseTable()
{

}

DataBaseTable::~DataBaseTable()
{

}

void DataBaseTable::addTableColumn(std::string fieldName, std::string fieldDataType)
{
    mDbTableFieldDetails.insert(std::pair<std::string, std::string>(fieldName, fieldDataType));
}

COMMON_DEFINITIONS::eSTATUS DataBaseTable::setup()
{
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}