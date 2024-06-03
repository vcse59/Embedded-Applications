#ifndef __ICONFIGURATION_MANAGEMENT_H__
#define __ICONFIGURATION_MANAGEMENT_H__

#include "Enumerations/ConfigurationEnumerations.h"
#include "Interface/IConfigParser.h"

namespace Configuration
{
    class IConfigurationManagement
    {
        public:
            virtual ~IConfigurationManagement(){}
            virtual Configuration::CONFIGURATION_STATUS parse() = 0;

        protected:
            Configuration::ICONFIG_PARSER_SHARED_PTR mConfigParser;
    };

    typedef std::shared_ptr<Configuration::IConfigurationManagement> ICONFIG_MGMT_SHARED_PTR;
}

#endif