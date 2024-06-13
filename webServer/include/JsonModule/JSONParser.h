#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#include <json/json.h>

#include "Interfaces/DataParserInteface.h"

namespace JSON_SERVICE{
    class jsonParser : public PARSER_INTERFACE::DataParserInterface
    {
        public:
            jsonParser(std::shared_ptr<std::string> data);
            ~jsonParser();

            void serialize() override;
            COMMON_DEFINITIONS::eSTATUS deserialize() override;
            std::string dumpData() override;

            static bool isStringValidJSON(std::string& jsonData)
            {
                Json::Value parsedRoot;
                Json::Reader reader;
                bool isParseSuccessful = reader.parse(jsonData, parsedRoot);

                return isParseSuccessful;
            }
        
        private:
            jsonParser(const jsonParser&) = delete;
            jsonParser(const jsonParser&&) = delete;
            jsonParser& operator=(const jsonParser&) = delete;
            jsonParser& operator=(const jsonParser&&) = delete;

            std::shared_ptr<Json::Value> m_SParsedRoot = nullptr;
            std::shared_ptr<std::string> m_Data;
    };
}

#endif