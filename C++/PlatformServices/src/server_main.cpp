#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <string>
#include <algorithm>
#include <future>
#include <termios.h>
#include <json/json.h>

#include "Modules/ConsoleMain.h"
#include "JsonModule/JSONParser.h"
#include "JsonModule/JsonItem.h"

using namespace std;
using namespace NetworkClass;
using namespace Storage;
using namespace COMMON_DEFINITIONS;

void userInput(NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE intf)
{
    std::cout << "Press any 'exit' to quit:" << std::endl;
    
    const char* exitStr = "exit";
    char input[5]; // Buffer to hold the input string
    int inputLength = 0;

    while (true) {
        // Check if a key has been pressed
        char key;
        if (read(STDIN_FILENO, &key, 1) != -1) {
            //std::cout << "You pressed: " << key << std::endl;
            if (key == '\n') {
                input[inputLength] = '\0'; // Null-terminate the input string
                if (strcmp(input, exitStr) == 0)
                    break; // Exit the loop if "exit" is entered
                inputLength = 0; // Reset the input buffer
            } else {
                // Append the character to the input buffer
                input[inputLength++] = key;
                if (inputLength == 5) {
                    std::cerr << "Input too long. Please enter 'exit' to quit." << std::endl;
                    inputLength = 0; // Reset the input buffer
                }
            }
        }
        
        // Do other tasks here...
        usleep(100000); // Sleep for 100 milliseconds (adjust as needed)
    }

    std::cout << "Exiting program..." << std::endl;
    intf->closeSocket();
}

int main(int argc, char *argv[])
{
    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ntwkInteface = consoleApp->getTCPServer();
    
    // Initialize the Database
    DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE dbConnector = consoleApp->getDBInstance();
    COMMON_DEFINITIONS::eSTATUS status = dbConnector->connectToDBService();

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS){
        std::cout << "Connected to MySQL" << std::endl;
    }
    else{
        std::cout << "Not connected" << std::endl;
    }

    status = dbConnector->createDataBase("Test1");

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        std::cout << "DB Created Successfully" << std::endl;
    }
    else
    {
        std::cout << "DB Creation failed" << std::endl;
    }

    
    // Define table name
    std::string tableName = "sampleTable";

    // Create table
    string createTableQuery = "CREATE TABLE " + tableName +" ("
                                "id INT AUTO_INCREMENT PRIMARY KEY,"
                                "SessionID VARCHAR(50),"
                                "Payload LONGTEXT"
                              ")";

    status == dbConnector->executeQuery(tableName, createTableQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_DEFINITION);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        std::cout << "Table created successfully" << std::endl;
    }
    else{
        std::cout << "Unable to create table" << std::endl;
    }

    // Start a thread that waits for user input
    std::thread userThread(userInput, ntwkInteface);

    // Start server
    std::thread th1(&NetworkClassInterface::createServer, ntwkInteface.get());    
    
    // Join the thread
    th1.join();
    userThread.join();
    

    
    /*std::shared_ptr<FRAMEWORK::ConsoleAppInterface> intf = FRAMEWORK::ConsoleMain::getConsoleAppInterface();

    DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE dbConnector = intf->getDBInstance();

    COMMON_DEFINITIONS::eSTATUS status = dbConnector->connectToDBService();

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS){
        std::cout << "Connected to MySQL" << std::endl;
    }
    else{
        std::cout << "Not connected" << std::endl;
    }

    status = dbConnector->createDataBase("Test1");

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        std::cout << "DB Created Successfully" << std::endl;
    }
    else
    {
        std::cout << "DB Creation failed" << std::endl;
    }

    
    // Define table name
    std::string tableName = "sampleTable";

    // Create table
    string createTableQuery = "CREATE TABLE " + tableName +" ("
                                "id INT AUTO_INCREMENT PRIMARY KEY,"
                                "name VARCHAR(50),"
                                "age INT"
                              ")";

    status == dbConnector->executeQuery(tableName, createTableQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_DEFINITION);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        std::cout << "Table created successfully" << std::endl;
    }
    else{
        std::cout << "Unable to create table" << std::endl;
    }

    //Insert table query
    std::string name = "Vivek";
    
    for (int i = 0; i < 10000; i++){
        std::string insertQuery = "INSERT INTO " + tableName + " (name ,age) VALUES (\"" + name + std::to_string(i + 1) + "\" , " + std::to_string(((i * 30) + 30) % 100) + ")";
        status == dbConnector->executeQuery(tableName, insertQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_MANIPULATION);
        if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
        {
            std::cout << "Record created successfully" << std::endl;
        }
        else{
            std::cout << "Unable to insert record" << std::endl;
        }
    }*/

    /*std::shared_ptr<std::string> jsonString = std::make_shared<std::string>("{\"name\": \"John\", \"age\": 30}");
    std::shared_ptr<PARSER_INTERFACE::DataParserInterface> parser = std::make_shared<JSON_SERVICE::jsonParser>(jsonString);
    parser->deserialize();
    std::cout << parser->dumpData() << std::endl;*/

    /*Json::Value root;
    root["name"] = "Vivek";
    Json::Value address;
    address["Street"] = "2667 N Snowberry Drive";
    address["City"] = "Saratoga Springs";
    address["State"] = "Utah";
    address["Pincode"] = 84045;
    address["Country"] = "United State Of America";
    root["Address"] = address;
    std::cout << root.toStyledString() << std::endl;*/

    /*JSON_SERVICE::JsonItem jsonRoot;
    jsonRoot.add("Name", "Vivek");
    jsonRoot.add("Age", 35);
    JSON_SERVICE::SJsonItem addressBlock = std::make_shared<JSON_SERVICE::JsonItem>();
    addressBlock->add("Street", "2667 N Snowberry Drive");
    addressBlock->add("City", "Saratoga Springs");
    addressBlock->add("State", "Utah");
    addressBlock->add("Pincode", 84045);
    addressBlock->add("Country", "United State Of America");
    jsonRoot.add("Address", addressBlock);
    jsonRoot.dump();*/

    /*SingleLinkedList<SingleLLNode>& singleLL = intf->getSingleLinkedList();
    unsigned long key1 = 0;
    unsigned long key2 = 0;

    unsigned int data1 = 10;
    SingleLLNode *node1 = new SingleLLNode((char*) &data1, sizeof(data1));
    singleLL.insertAtEnd(node1);
    key2 = node1->getKey();

    singleLL.printLinkedList();
    */
    return 0;
}
