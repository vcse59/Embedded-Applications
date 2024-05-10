#include <iostream>
#include "JsonModule/JsonItem.h"

using namespace JSON_SERVICE;

JsonItem::JsonItem(LOGGER_SERVICE::S_PTR_LOGGER logger):m_logger(logger)
{
    SJsonRoot = std::make_shared<Json::Value>();
}

JsonItem::~JsonItem()
{}

SJsonItem JsonItem::add(std::string fieldName, int fieldValue)
{
    (*SJsonRoot)[fieldName] = Json::Value(fieldValue);
    return std::make_shared<JsonItem>(*this);
}

SJsonItem JsonItem::add(std::string fieldName, float fieldValue)  
{
    (*SJsonRoot)[fieldName] = Json::Value(fieldValue);
    return std::make_shared<JsonItem>(*this);
}

SJsonItem JsonItem::add(std::string fieldName, double fieldValue)  
{
    (*SJsonRoot)[fieldName] = Json::Value(fieldValue);
    return std::make_shared<JsonItem>(*this);
}

SJsonItem JsonItem::add(std::string fieldName, long fieldValue)
{
    (*SJsonRoot)[fieldName] = Json::Value(fieldValue);
    return std::make_shared<JsonItem>(*this);
}

SJsonItem JsonItem::add(std::string fieldName, bool fieldValue)  
{
    (*SJsonRoot)[fieldName] = Json::Value(fieldValue);
    return std::make_shared<JsonItem>(*this);
}

SJsonItem JsonItem::add(std::string fieldName, std::string fieldValue)  
{
    (*SJsonRoot)[fieldName] = Json::Value(fieldValue);
    return std::make_shared<JsonItem>(*this);
}

SJsonItem JsonItem::add(std::string fieldName, const char* fieldValue)  
{
    (*SJsonRoot)[fieldName] = Json::Value(std::string(fieldValue));
    return std::make_shared<JsonItem>(*this);
}

SJsonItem JsonItem::add(std::string fieldName, SJsonItem fieldValue)
{
    (*SJsonRoot)[fieldName] = *(fieldValue->SJsonRoot);
    return std::make_shared<JsonItem>(*this);
}

void JsonItem::dump()
{
    std::cout << SJsonRoot->toStyledString() << std::endl;
}
