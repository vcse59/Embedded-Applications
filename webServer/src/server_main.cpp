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
#include "Containers/Queue.h"

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
    COMMON_DEFINITIONS::eSTATUS status = FRAMEWORK::ConsoleMain::Initialize();

    if (status != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        std::cerr << "Initialization Failed for application...Exiting" << std::endl;
        return -1;
    }

    FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
    NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ntwkInteface = consoleApp->getTCPServer();
    
    // Initialize the Database
    DATABASE_SERVICE::S_PTR_DATABASE_CONNECTOR_INTERFACE dbConnector = consoleApp->getDBInstance();    

    // Start a thread that waits for user input
    std::thread userThread(userInput, ntwkInteface);

    // Start server
    std::thread th1(&NetworkClassInterface::createServer, ntwkInteface.get(), NetworkClass::eLISTENING_MODE::EPOLL_MODE);    
    
    // Join the thread
    th1.join();
    userThread.join();
    return 0;
}
