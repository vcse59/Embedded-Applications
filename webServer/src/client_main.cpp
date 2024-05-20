#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <string>
#include <algorithm>
#include <future>
#include <termios.h>

#include "Modules/TCPService/TCPClient.h"

using namespace std;
using namespace NetworkClass;
using namespace COMMON_DEFINITIONS;

int main(int argc, char *argv[])
{

    NetworkClass::S_PTR_NETWORK_CLASS_INTERFACE ntwkInteface = std::make_shared<NetworkClass::TCPClient>("127.0.0.1", 9999);

    // Start server
    std::thread th1(&NetworkClassInterface::connectToServer, ntwkInteface.get());    
    
    // Join the thread
    th1.join();

    return 0;
}
