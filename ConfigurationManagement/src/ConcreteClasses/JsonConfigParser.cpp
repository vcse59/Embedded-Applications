#include "ConcreteClasses/JsonConfigParser.h"

using namespace Configuration;

JsonConfigParser::JsonConfigParser()
{

}

JsonConfigParser::~JsonConfigParser()
{

}

Configuration::CONFIGURATION_STATUS JsonConfigParser::parse()
{
    std::cout << "Parsing Json Configuration file" << std::endl;
    return Configuration::CONFIGURATION_STATUS::CONFIGURATION_READ_PARSING_SUCCESS;
}