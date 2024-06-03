#ifndef __XML_CONFIG_PARSER_H__
#define __XML_CONFIG_PARSER_H__

#include "Interface/IConfigParser.h"

namespace Configuration
{
    class XMLConfigParser : public Configuration::IConfigParser
    {
    public:
        XMLConfigParser();
        ~XMLConfigParser();

        Configuration::CONFIGURATION_STATUS parse() override;
    };
}

#endif