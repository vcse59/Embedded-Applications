#include "Modules/Logger/RemoteWriter.h"

using namespace LOGGER_SERVICE;

COMMON_DEFINITIONS::eSTATUS RemoteWriter::connectToRemoteLogServer()
{
    struct sockaddr_in ServerAddress;

    // Create a socket
    if ((mClientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Socket creation failed" << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::SOCKET_INITIALIZATION_FAILED;
    }

    // Initialize server address structure
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(mPortNumber);
    ServerAddress.sin_addr.s_addr = inet_addr(mServerIPAddress.c_str());

    // Connect to server
    if (connect(mClientSocket, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress)) == -1)
    {
        closeSocket();
        return COMMON_DEFINITIONS::eSTATUS::SOCKET_INITIALIZATION_FAILED;
    }

    std::cout << "Connection to server : " << mServerIPAddress << " at PORT : " << mPortNumber << " is CONNECTED" << std::endl;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS RemoteWriter::sendMessage(const std::string &message)
{
    // Send HTTP response
    const char *data = message.c_str();
    size_t remaining = strlen(data);
    while (remaining > 0)
    {
        ssize_t sent = send(mClientSocket, data, remaining, 0);
        if (sent == -1)
        {
            // Handle send error
            std::cerr << "Error sending data\n";
            closeSocket();
            return COMMON_DEFINITIONS::eSTATUS::ERROR;
        }
        else if (sent == 0)
        {
            // Connection closed by peer
            std::cerr << "Connection closed by peer\n";
            closeSocket();
            return COMMON_DEFINITIONS::eSTATUS::ERROR;
        }
        else
        {
            // Advance buffer pointer and update remaining data size
            data += sent;
            remaining -= sent;
        }
    }

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS RemoteWriter::closeSocket()
{
    if (mClientSocket != -1)
        close(mClientSocket);
    mClientSocket = -1;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

void RemoteWriter::processLogMessage()
{
    while (true)
    {
        std::unique_lock<std::mutex> lck(mMutex);
        while (!readyToProcess)
            mNotifyConsumer.wait(lck);

        FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
        EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE queueInterface = consoleApp->getLoggerQueueInterface();
        EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE pendingQueueInterface = consoleApp->getLoggerPendingQueueInterface();

        while (pendingQueueInterface->getQueueLength() > 0 && mClientSocket != -1)
        {
            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> pendingEventMessage = pendingQueueInterface->getEvent();
            EVENT_MESSAGE::LoggerMessage *pendingMessage = (EVENT_MESSAGE::LoggerMessage *)pendingEventMessage->getEventData();
            sendMessage(std::string(pendingMessage->mLoggerString));
        }

        while (queueInterface->getQueueLength() > 0)
        {
            std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> elem1 = queueInterface->getEvent();
            EVENT_MESSAGE::LoggerMessage *message = (EVENT_MESSAGE::LoggerMessage *)elem1->getEventData();
            if (mClientSocket != -1){
                sendMessage(std::string(message->mLoggerString));
            }
            else{
                std::cout << message->mLoggerString;

                EVENT_MESSAGE::LoggerMessage pendingMessage;
                memcpy(pendingMessage.mLoggerString, message->mLoggerString, sizeof(pendingMessage));
                pendingMessage.mMessageLen = message->mMessageLen;
                std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> event = std::make_shared<EVENT_MESSAGE::LoggerEventMessage>();
                event->setMessage(reinterpret_cast<char *>(&pendingMessage), sizeof(pendingMessage));
                pendingQueueInterface->pushEvent(event);
            }
        }
        mNotifyProducer.notify_one();
        readyToProcess = false;
    }
}

void RemoteWriter::retryConnection()
{
    while(true){
        if (mClientSocket == -1){
            if (connectToRemoteLogServer() != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
            {
                std::cout << "Retrying to connect again in 5 secs to log server IP " << mServerIPAddress << " at port number " << mPortNumber << std::endl; 
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }else{
                std::cout << "Successfully connected to log server IP " << mServerIPAddress << " at port number " << mPortNumber << std::endl;

                std::unique_lock<std::mutex> lck(mMutex);

                FRAMEWORK::S_PTR_CONSOLEAPPINTERFACE consoleApp = FRAMEWORK::ConsoleMain::getConsoleAppInterface();
                EVENT_MESSAGE::S_PTR_EVENT_QUEUE_INTERFACE pendingQueueInterface = consoleApp->getLoggerPendingQueueInterface();

                while (pendingQueueInterface->getQueueLength() > 0)
                {
                    std::shared_ptr<EVENT_MESSAGE::EventMessageInterface> pendingEventMessage = pendingQueueInterface->getEvent();
                    EVENT_MESSAGE::LoggerMessage *pendingMessage = (EVENT_MESSAGE::LoggerMessage *)pendingEventMessage->getEventData();
                    sendMessage(std::string(pendingMessage->mLoggerString));
                }
            }
        }
    }
}