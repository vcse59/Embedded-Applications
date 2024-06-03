#include "ConcreteClasses/XMLConfigParser.h"

using namespace Configuration;

XMLConfigParser::XMLConfigParser()
{
}

XMLConfigParser::~XMLConfigParser()
{
}

Configuration::CONFIGURATION_STATUS XMLConfigParser::parse()
{
    std::cout << "Parsing XML Configuration" << std::endl;
    return Configuration::CONFIGURATION_STATUS::CONFIGURATION_READ_PARSING_SUCCESS;
}