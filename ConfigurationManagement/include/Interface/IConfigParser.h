#ifndef __ICONFIG_PARSER_H__
#define __ICONFIG_PARSER_H__

#include "Enumerations/ConfigurationEnumerations.h"

namespace Configuration
{
    class IConfigParser
    {
        public:
            virtual ~IConfigParser() {}
            virtual Configuration::CONFIGURATION_STATUS parse() = 0;
    };

    typedef std::shared_ptr<Configuration::IConfigParser> ICONFIG_PARSER_SHARED_PTR;
} // namespace ConfigurationManagement

#endif