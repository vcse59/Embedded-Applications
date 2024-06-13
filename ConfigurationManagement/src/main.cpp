#include <iostream>
#include "json/json.h"

#include "ConcreteClasses/ConfigurationManagement.h"
#include "ConcreteClasses/JsonConfigParser.h"
#include "ConcreteClasses/XMLConfigParser.h"

using namespace Configuration;

int main(int argc, char *argv[])
{
    Json::Value root;
    Json::Value data;
    constexpr bool shouldUseOldWay = false;
    root["action"] = "run";
    data["number"] = 1;
    root["data"] = data;

    if (shouldUseOldWay)
    {
        Json::FastWriter writer;
        const std::string json_file = writer.write(root);
        std::cout << json_file << std::endl;
    }
    else
    {
        Json::StreamWriterBuilder builder;
        const std::string json_file = Json::writeString(builder, root);
        std::cout << json_file << std::endl;
    }

    ICONFIG_PARSER_SHARED_PTR configParser = std::make_shared<XMLConfigParser>();
    ICONFIG_MGMT_SHARED_PTR configMgmt = std::make_shared<ConfigurationManagement>(configParser);

    configMgmt->parse();
}
