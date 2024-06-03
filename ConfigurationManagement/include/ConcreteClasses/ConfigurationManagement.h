#ifndef __CONFIGURATION_MANAGEMENT_H__
#define __CONFIGURATION_MANAGEMENT_H__

#include "Interface/IConfigurationManagement.h"

namespace Configuration
{
    class ConfigurationManagement : public Configuration::IConfigurationManagement
    {
        public:
            ConfigurationManagement();
            ~ConfigurationManagement();
            ConfigurationManagement(Configuration::ICONFIG_PARSER_SHARED_PTR ConfigParser);
            Configuration::CONFIGURATION_STATUS parse() override;
    };
}

#endif