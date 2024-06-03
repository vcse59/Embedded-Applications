#ifndef __JSON_CONFIG_PARSER_H__
#define __JSON_CONFIG_PARSER_H__

#include "Interface/IConfigParser.h"

namespace Configuration
{
    class JsonConfigParser : public Configuration::IConfigParser
    {
        public:
            JsonConfigParser();
            ~JsonConfigParser();

            Configuration::CONFIGURATION_STATUS parse() override;
    };
}

#endif