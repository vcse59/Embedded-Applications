#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <iostream>
#include <functional>

#include "CommonClasses/CommonDefinitions.h"

using namespace COMMON_DEFINITIONS;
using namespace std;

namespace Storage
{
    // Class to keep generic data
    class SingleLLNode
    {
    public:
        // Parameterized Constructor
        explicit SingleLLNode(char* data, unsigned int dataLength, std::function<std::string()> funcCallback)
        {
            mCallback = funcCallback;
            mData = new char[dataLength + 1];
            setData(data, dataLength);
        }

        // Parameterized Constructor
        explicit SingleLLNode(char* data, unsigned int dataLength)
        {
            mData = new char[dataLength + 1];
            setData(data, dataLength);
        }

        // Default Constructor
        SingleLLNode() {}

        // Destructor
        ~SingleLLNode()
        {
            if (mData != NULL)
            {
                delete[] mData;
            }

            mData = NULL;
            mKey = 0;
            mDataLength = 0;
            mNext = NULL;
        }

        // Copy Constructor
        SingleLLNode(const SingleLLNode &instance)
        {
            std::lock_guard<std::mutex> lock(mMutex);
            if (mData != NULL)
            {
                delete[] mData;
                mData = NULL;
                mDataLength = 0;
                mKey = 0;
                mNext = NULL;
            }
            
            mKey = generateKey();
            mNext = NULL;
            mCallback = instance.mCallback;
            mDataLength = instance.getDataLength();
            mData = new char[mDataLength + 1];
            memcpy(mData, instance.getData(), mDataLength);
            mData[mDataLength] = '\0';
        }

        // Assignment operator
        SingleLLNode &operator=(const SingleLLNode &instance)
        {
            std::lock_guard<std::mutex> lock(mMutex);
            if (mData != nullptr)
            {
                delete[] mData;
                mData = NULL;
                mDataLength = 0;
                mKey = 0;
                mNext = NULL;
            }

            mKey = generateKey();
            mCallback = instance.mCallback;
            mNext = NULL;
            mDataLength = instance.getDataLength();
            mData = new char[mDataLength + 1];
            memcpy(mData, instance.getData(), mDataLength);
            mData[mDataLength] = '\0';
            return *this;
        }

        // Setting next node memory address
        void setNext(SingleLLNode *node) { mNext = node; }

        // Retrieving next node memory address
        SingleLLNode *getNext() { return mNext; }

        // Set data
        void setData(char* data, unsigned int dataLength)
        {
            if (mData != nullptr)
            {
                delete[] mData;
                mData = nullptr;
            }

            mData = new char[dataLength + 1];
            memset(mData, 0, dataLength);
            mKey = generateKey();
            memcpy(mData, data, dataLength);
            mDataLength = dataLength;
            mData[dataLength] = '\0';
        }

        int generateKey() const 
        {
            std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
            std::chrono::system_clock::duration dtn = tp.time_since_epoch();
            return dtn.count();
        }

        // Get Data
        char *getData() const { return mData; }

        // Get Key
        unsigned long getKey() const { return mKey; }

        // Get Data Length
        unsigned int getDataLength() const { return mDataLength; }

        // Print data
        std::string printData(){
            if (!mCallback)
            {
                return "";
            }

            return mCallback();
        }

    private:
        std::function<std::string()> mCallback;
        char *mData = nullptr;
        unsigned int mDataLength = {0};
        unsigned long mKey = {0};
        std::mutex mMutex;
        Storage::SingleLLNode *mNext = NULL;
        //SingleLLNode(const SingleLLNode &&) = delete;
        //SingleLLNode &operator=(const SingleLLNode &&) = delete;
    };

    typedef std::shared_ptr<Storage::SingleLLNode> S_PTR_SINGLELL_NODE;

    template<class T>
    class SingleLinkedList{

        public:
            SingleLinkedList();
            ~SingleLinkedList();

            COMMON_DEFINITIONS::eSTATUS insertAtEnd(T item);
            COMMON_DEFINITIONS::eSTATUS insertAtStart(T item);
            COMMON_DEFINITIONS::eSTATUS insertAtNth(unsigned int nthPosition, T item);

            void reverse();
            void deleteFront();
            void deleteBack();
            void deleteNth(unsigned int nthPosition);
            void deleteByKey(unsigned long pKey);

            T popFront();
            T popBack();

            T search(unsigned long pKey);

            COMMON_DEFINITIONS::eSTATUS clear();
            void printLinkedList();
            unsigned int getLength() const {return mNodeCount;}
        private:
            unsigned int mNodeCount = 0;
            std::mutex mMutex;
            T *mHeadNode = nullptr;

            SingleLinkedList(const SingleLinkedList&) = delete;
            SingleLinkedList& operator=(const SingleLinkedList&) = delete;
            SingleLinkedList(const SingleLinkedList&&) = delete;
            SingleLinkedList& operator=(const SingleLinkedList&&) = delete;
    };
}

template <class T>
Storage::SingleLinkedList<T>::SingleLinkedList()
{
}

template <class T>
Storage::SingleLinkedList<T>::~SingleLinkedList()
{
    this->clear();
}

template <class T>
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::insertAtEnd(T item)
{
    std::lock_guard<std::mutex> lock(mMutex);
    T *newNode = new T();
    *newNode = item;

    // If head node is empty then assign new node to head node
    if (mHeadNode == NULL)
    {
        mHeadNode = newNode;
    }
    else
    {
        T *tempNode = mHeadNode;

        while (tempNode->getNext() != NULL)
        {
            tempNode = tempNode->getNext();
        }

        tempNode->setNext(newNode);
    }
    mNodeCount++;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

template <class T>
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::insertAtStart(T item)
{
    std::lock_guard<std::mutex> lock(mMutex);
    T *newNode = new T();
    *newNode = item;

    // If head node is empty then assign new node to head node
    if (mHeadNode == NULL)
    {
        mHeadNode = newNode;
    }
    else
    {
        newNode->setNext(mHeadNode);
        mHeadNode = newNode;
    }
    mNodeCount++;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

template <class T>
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::insertAtNth(unsigned int nthPosition, T item)
{
    std::lock_guard<std::mutex> lock(mMutex);
    T *newNode = new T();
    *newNode = item;
    unsigned int indexCount = 0;
    T* tempNode = mHeadNode;
    T* prevNode = NULL;
    while ((indexCount < (nthPosition - 1)) && (tempNode != NULL))
    {
        indexCount++;
        prevNode = tempNode;
        tempNode = tempNode->getNext();
    }

    if (indexCount !=  (nthPosition - 1))
        return COMMON_DEFINITIONS::eSTATUS::ERROR;

    newNode->setNext(tempNode);
    if (prevNode)
        prevNode->setNext(newNode);
    else
        mHeadNode = newNode;
    mNodeCount++;

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

template <class T>
void Storage::SingleLinkedList<T>::reverse()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mHeadNode == NULL)
    {
        return;
    }

    T* tempNode = mHeadNode;
    T* tempHeadNode = nullptr;

    while (tempNode != nullptr)
    {
        T* nextNode = tempNode->getNext();
        if (tempHeadNode == nullptr){
            tempHeadNode = tempNode;
            tempHeadNode->setNext(nullptr);
        }
        else{
            tempNode->setNext(tempHeadNode);
            tempHeadNode = tempNode;
        }
        tempNode = nextNode;
    }
    mHeadNode = tempHeadNode;
}

template <class T>
void Storage::SingleLinkedList<T>::deleteFront()
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mHeadNode == NULL)
    {
        return;
    }
    T* tempNode = mHeadNode;
    mHeadNode = mHeadNode->getNext();

    if (tempNode != NULL)
    {
        delete tempNode;
    }
    tempNode = NULL;
    mNodeCount--;
}

template <class T>
void Storage::SingleLinkedList<T>::deleteBack()
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mHeadNode == NULL)
    {
        return;
    }
    T *tempNode = mHeadNode;
    T* prevNode = NULL;

    if (tempNode == NULL)
        return;

    while (tempNode->getNext() != NULL)
    {
        prevNode = tempNode;
        tempNode = tempNode->getNext();
    }

    if (prevNode == NULL)
    {
        delete mHeadNode;
        mHeadNode = NULL;
    }
    else
    {
        prevNode->setNext(NULL);
        delete tempNode;
    }
    mNodeCount--;
}

template <class T>
void Storage::SingleLinkedList<T>::deleteNth(unsigned int nthPosition)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if ((mHeadNode == NULL) || (nthPosition <= 0))
    {
        return;
    }

    if (nthPosition == 1)
    {
        T* tempNode = mHeadNode;
        mHeadNode = mHeadNode->getNext();
        delete tempNode;
        return;
    }

    int indexCount = 0;
    T* tempNode = mHeadNode;
    T* prevNode = mHeadNode;
    while ( (indexCount < (nthPosition - 1)) && (tempNode != NULL))
    {
        prevNode = tempNode;
        tempNode = tempNode->getNext();
        indexCount++;
    }

    if (tempNode != NULL)
    {
        prevNode->setNext(tempNode->getNext());
        delete tempNode;
    }
    mNodeCount--;
}

template <class T>
void Storage::SingleLinkedList<T>::deleteByKey(unsigned long pKey)
{
    std::lock_guard<std::mutex> lock(mMutex);
    T *tempNode = mHeadNode;
    T *prevNode = NULL;

    if (mHeadNode == NULL){
        return;
    }

    while ((tempNode != NULL) && (pKey != tempNode->getKey()))
    {
        prevNode = tempNode;
        tempNode = tempNode->getNext();
    }

    if (tempNode != NULL)
    {
        prevNode->setNext(tempNode->getNext());
        delete tempNode;
        mNodeCount--;
    }
}

template <class T>
T Storage::SingleLinkedList<T>::popFront()
{
    std::lock_guard<std::mutex> lock(mMutex);
    T output;
    if (mHeadNode == NULL){
        return output;
    }

    T *tempNode = mHeadNode;
    mHeadNode = mHeadNode->getNext();
    output = *tempNode;
    delete tempNode;
    mNodeCount--;
    return output;
}

template <class T>
T Storage::SingleLinkedList<T>::popBack()
{
    std::lock_guard<std::mutex> lock(mMutex);
    T output;

    if (mHeadNode == NULL){
        return output;
    }

    T *tempNode = mHeadNode;
    T *prevNode = mHeadNode;

    while (tempNode->getNext() != NULL)
    {
        prevNode = tempNode;
        tempNode = tempNode->getNext();
    }

    if (prevNode == mHeadNode)
    {
        delete mHeadNode;
        mHeadNode = NULL;
    }
    else
    {
        prevNode->setNext(NULL);
        output = *tempNode;
        delete tempNode;
    }
    mNodeCount--;
    return output;
}

template <class T>
T Storage::SingleLinkedList<T>::search(unsigned long pKey)
{
    std::lock_guard<std::mutex> lock(mMutex);
    T *tempNode = mHeadNode;

    while ((tempNode != NULL) && (pKey != tempNode->getKey()))
    {
        tempNode = tempNode->getNext();
    }

    if (tempNode == NULL){
        return T();
    }
    return *tempNode;
}

template <class T>
void Storage::SingleLinkedList<T>::printLinkedList()
{
    std::lock_guard<std::mutex> lock(mMutex);
    T *tempNode = mHeadNode;

    if (tempNode == NULL)
    {
        cout << "No Item is left in Linked List Container" << endl;
        return;
    }

    while (tempNode != NULL)
    {
        unsigned long lKey = tempNode->getKey();
        cout << lKey << " -> " << tempNode->printData() << " ";
        tempNode = tempNode->getNext();
    }
    cout << endl;
}

template <class T>
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::clear()
{
    std::lock_guard<std::mutex> lock(mMutex);
    T *tempNode = mHeadNode;
    T *temp = NULL;

    while (tempNode != NULL)
    {
        temp = tempNode->getNext();
        delete tempNode;
        tempNode = temp;
    }
    cout << endl;

    mHeadNode = tempNode;
    mNodeCount = 0;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

#endif