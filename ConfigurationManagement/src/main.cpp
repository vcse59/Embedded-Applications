#include <iostream>
#include "ConcreteClasses/ConfigurationManagement.h"
#include "ConcreteClasses/JsonConfigParser.h"
#include "ConcreteClasses/XMLConfigParser.h"

using namespace Configuration;

int main(int argc, char *argv[])
{
    ICONFIG_PARSER_SHARED_PTR configParser = std::make_shared<XMLConfigParser>();
    ICONFIG_MGMT_SHARED_PTR configMgmt = std::make_shared<ConfigurationManagement>(configParser);

    configMgmt->parse();
}
