#include "ConcreteClasses/ConfigurationManagement.h"

using namespace Configuration;

ConfigurationManagement::ConfigurationManagement(Configuration::ICONFIG_PARSER_SHARED_PTR ConfigParser)
{
    mConfigParser = ConfigParser;
}

ConfigurationManagement::~ConfigurationManagement()
{

}

Configuration::CONFIGURATION_STATUS ConfigurationManagement::parse()
{
    mConfigParser->parse();
    return Configuration::CONFIGURATION_STATUS::CONFIGURATION_READ_PARSING_SUCCESS;
}
