#include "Modules/Logger/RemoteWriter.h"
#include <signal.h>
#include <pthread.h>

using namespace LOGGER_SERVICE;

COMMON_DEFINITIONS::eSTATUS RemoteWriter::connectToRemoteLogServer()
{
    struct sockaddr_in ServerAddress;
    int opt = -1;

    // Create a socket
    if ((mClientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Socket creation failed" << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::SOCKET_INITIALIZATION_FAILED;
    }

    // set master socket to allow multiple connections ,
    // this is just a good habit, it will work without this
    if (setsockopt(mClientSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        perror("setsockopt");
        closeSocket();
        return COMMON_DEFINITIONS::eSTATUS::SOCKET_INITIALIZATION_FAILED;
    }

    // Initialize server address structure
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(mPortNumber);
    ServerAddress.sin_addr.s_addr = inet_addr(mServerIPAddress.c_str());

    // Connect to server
    if (connect(mClientSocket, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress)) == -1)
    {
        std::cout << "Connect failed" << std::endl;
        closeSocket();
        return COMMON_DEFINITIONS::eSTATUS::SOCKET_INITIALIZATION_FAILED;
    }

    std::cout << "Connected to server : " << mServerIPAddress << " at PORT : " << mPortNumber << " is CONNECTED" << std::endl;
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
            if (errno == EPIPE || errno == ECONNRESET)
            {
                std::cerr << "Connection closed by the server." << std::endl;
            }
            else
            {
                std::cerr << "Failed to send data: " << strerror(errno) << std::endl;
            }
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

void RemoteWriter::processLogEvents()
{
    // Block SIGPIPE in this thread
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &set, NULL);


    std::weak_ptr<FRAMEWORK::ConsoleAppInterface> weakPtr(FRAMEWORK::ConsoleMain::getConsoleAppInterface());
    std::weak_ptr<EVENT_MESSAGE::EventQueueInterface> eventWeakPtr(weakPtr.lock()->getLoggerQueueInterface());
    std::weak_ptr<EVENT_MESSAGE::EventQueueInterface> pendingQueueWeakPtr(weakPtr.lock()->getLoggerPendingQueueInterface());

    auto queueInterface = eventWeakPtr.lock();
    auto pendingQueueInterface = pendingQueueWeakPtr.lock();

    while (true)
    {
        std::unique_lock<std::mutex> lck(mMutex);
        mNotifyLoggerThread.wait(lck);

        if (mNeedToClose)
        {
            std::cout << "Shutting down the remote logger thread";
            break;
        }

        if (queueInterface->getQueueLength() > 0)
        {
            EVENT_MESSAGE::EventMessageInterface elem1 = queueInterface->getEvent();
            EVENT_MESSAGE::LoggerMessage *message = (EVENT_MESSAGE::LoggerMessage *)elem1.getEventData();
            if (mClientSocket != -1)
            {
                if (sendMessage(std::string(message->mLoggerString)) != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
                {
                    std::unique_lock<std::mutex> retryLock(mRetryMutex);
                    mRetryCV.notify_one();
                }
            }
            else{
                std::cout << message->mLoggerString;

                EVENT_MESSAGE::LoggerMessage pendingMessage;
                memcpy(pendingMessage.mLoggerString, message->mLoggerString, sizeof(pendingMessage));
                pendingMessage.mMessageLen = message->mMessageLen;
                EVENT_MESSAGE::LoggerEventMessage event;
                event.setMessage(reinterpret_cast<char *>(&pendingMessage), sizeof(pendingMessage));
                pendingQueueInterface->pushEvent(event);
            }
        }
    }
}

void RemoteWriter::retryConnection()
{
    // Block SIGPIPE in this thread
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    while(true){
        std::unique_lock<std::mutex> lck(mRetryMutex);
        if (mRetryCV.wait_for(lck, std::chrono::seconds(COMMON_DEFINITIONS::LOGGER_SERVER_CONNECTION_RETRY_SECONDS)) == std::cv_status::timeout)//, [&]{return (mClientSocket == -1);}))
        {
            if (mNeedToClose)
            {
                std::cout << "Shutting down the retry thread (to reconnect to logger server)" << std::endl;
                break;
            }

            if (sendMessage(COMMON_DEFINITIONS::HEARTBEAT_STRING) != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
            {
                std::cout << "Trying to reconnect again" << std::endl;
                COMMON_DEFINITIONS::eSTATUS status = connectToRemoteLogServer();
                if (status != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
                {
                    std::cout << "Retrying to connect again in 5 secs to log server IP " << mServerIPAddress << " at port number " << mPortNumber << std::endl; 
                }else{
                    std::cout << "Successfully connected to log server IP " << mServerIPAddress << " at port number " << mPortNumber << std::endl;

                    std::unique_lock<std::mutex> lck(mMutex);

                    std::weak_ptr<FRAMEWORK::ConsoleAppInterface> weakPtr(FRAMEWORK::ConsoleMain::getConsoleAppInterface());
                    std::weak_ptr<EVENT_MESSAGE::EventQueueInterface> pendingQueueWeakPtr(weakPtr.lock()->getLoggerPendingQueueInterface());
                    auto pendingQueueInterface = pendingQueueWeakPtr.lock();

                    while (pendingQueueInterface->getQueueLength() > 0)
                    {
                        EVENT_MESSAGE::EventMessageInterface pendingEventMessage = pendingQueueInterface->getEvent();
                        EVENT_MESSAGE::LoggerMessage *pendingMessage = (EVENT_MESSAGE::LoggerMessage *)pendingEventMessage.getEventData();
                        sendMessage(std::string(pendingMessage->mLoggerString));
                    }
                }
            }
        }
    }
}