#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <iostream>
#include "CommonClasses/CommonDefinitions.h"
#include "Adapters/ConsoleAppInterface.h"

using namespace COMMON_DEFINITIONS;
using namespace std;

namespace Storage
{
    template<class T>
    class SingleLinkedList{

        public:
            SingleLinkedList(LOGGER_SERVICE::S_PTR_LOGGER logger):m_logger(logger){}
            SingleLinkedList();
            ~SingleLinkedList();

            COMMON_DEFINITIONS::eSTATUS insertAtEnd(T *item);
            COMMON_DEFINITIONS::eSTATUS insertAtStart(T *item);
            COMMON_DEFINITIONS::eSTATUS insertAtNth(unsigned int nthPosition, T *item);

            void deleteFront();
            void deleteBack();
            void deleteNth(unsigned int nthPosition);
            void deleteByKey(unsigned long pKey);

            T popFront();
            T popBack();

            T search(unsigned long pKey);

            COMMON_DEFINITIONS::eSTATUS clear();
            void printLinkedList();
        private:
            T *mHeadNode = NULL;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger = nullptr;
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
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::insertAtEnd(T *item)
{
    // If head node is empty then assign new node to head node
    if (mHeadNode == NULL)
    {
        mHeadNode = item;
    }
    else
    {
        T *tempNode = mHeadNode;

        while (tempNode->getNext() != NULL)
        {
            tempNode = tempNode->getNext();
        }

        tempNode->setNext(item);
    }

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

template <class T>
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::insertAtStart(T *item)
{
    // If head node is empty then assign new node to head node
    if (mHeadNode == NULL)
    {
        mHeadNode = item;
    }
    else
    {
        item->setNext(mHeadNode);
        mHeadNode = item;
    }

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

template <class T>
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::insertAtNth(unsigned int nthPosition, T *item)
{
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

    item->setNext(tempNode);
    if (prevNode)
        prevNode->setNext(item);
    else
        mHeadNode = item;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

template <class T>
void Storage::SingleLinkedList<T>::deleteFront()
{
    if (mHeadNode == NULL)
        return;
    T* tempNode = mHeadNode;
    mHeadNode = mHeadNode->getNext();

    if (tempNode != NULL)
    {
        delete tempNode;
    }
    tempNode = NULL;
}

template <class T>
void Storage::SingleLinkedList<T>::deleteBack()
{
    if (mHeadNode == NULL)
        return;
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
}

template <class T>
void Storage::SingleLinkedList<T>::deleteNth(unsigned int nthPosition)
{
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
}

template <class T>
void Storage::SingleLinkedList<T>::deleteByKey(unsigned long pKey)
{
    T *tempNode = mHeadNode;
    T *prevNode = NULL;

    if (mHeadNode == NULL)
        return;

    while ((tempNode != NULL) && (pKey != tempNode->getKey()))
    {
        prevNode = tempNode;
        tempNode = tempNode->getNext();
    }

    if (tempNode != NULL)
    {
        prevNode->setNext(tempNode->getNext());
        delete tempNode;
    }
}

template <class T>
T Storage::SingleLinkedList<T>::popFront()
{
    T output;
    if (mHeadNode == NULL)
        return output;

    T *tempNode = mHeadNode;
    mHeadNode = mHeadNode->getNext();
    output = *tempNode;
    delete tempNode;

    return output;
}

template <class T>
T Storage::SingleLinkedList<T>::popBack()
{
    T output;

    if (mHeadNode == NULL)
        return output;

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

    return output;
}

template <class T>
T Storage::SingleLinkedList<T>::search(unsigned long pKey)
{
    T *tempNode = mHeadNode;

    while ((tempNode != NULL) && (pKey != tempNode->getKey()))
    {
        tempNode = tempNode->getNext();
    }

    if (tempNode == NULL)
        return T();

    return *tempNode;
}

template <class T>
void Storage::SingleLinkedList<T>::printLinkedList()
{
    T *tempNode = mHeadNode;

    if (tempNode == NULL)
    {
        cout << "No Item is left in Linked List Container" << endl;
        return;
    }

    while (tempNode != NULL)
    {
        unsigned long lKey = tempNode->getKey();
        cout << lKey << " ";
        tempNode = tempNode->getNext();
    }
    cout << endl;
}

template <class T>
COMMON_DEFINITIONS::eSTATUS Storage::SingleLinkedList<T>::clear()
{
    cout << "Clearing the linked list" << endl;
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

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

#endif