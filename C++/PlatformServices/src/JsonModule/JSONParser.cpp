#include <iostream>
#include "JsonModule/JSONParser.h"

using namespace std;
using namespace JSON_SERVICE;

jsonParser::jsonParser(LOGGER_SERVICE::S_PTR_LOGGER logger, std::shared_ptr<std::string> data)
    :m_logger(logger)
{
    m_Data  = data;
}

jsonParser::~jsonParser()
{
}

void jsonParser::serialize()
{
    // Convert JSON object to string
    Json::StreamWriterBuilder writer;
    *m_Data = Json::writeString(writer, *m_SParsedRoot);
}

COMMON_DEFINITIONS::eSTATUS jsonParser::deserialize()
{
    // Convert string to JSON Object
    m_SParsedRoot = std::make_shared<Json::Value>();
    Json::Reader reader;
    if (!reader.parse(*m_Data, *m_SParsedRoot))
    {
        cout << "Failed to parse Json" << endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

std::string jsonParser::dumpData()
{
    return m_SParsedRoot->toStyledString();
}