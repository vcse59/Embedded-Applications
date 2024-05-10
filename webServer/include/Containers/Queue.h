#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "Containers/LinkedList.h"

namespace Storage
{
    template<class T>
    class QueueContainer
    {
        public:

            QueueContainer(LOGGER_SERVICE::S_PTR_LOGGER logger):m_logger{logger}{}
            ~QueueContainer(){}

            void enqueue(T* item){mContainer.insertAtEnd(item);}
            T dequeue(){return mContainer.popFront();}
            void search(unsigned long pKey) { return mContainer.search(pKey); }
            void remove(unsigned long pKey){}
            void clear(){mContainer.clear();}

        private:
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            Storage::SingleLinkedList<T> mContainer;
            QueueContainer(const QueueContainer&) = delete;
            QueueContainer& operator=(const QueueContainer&) = delete;
            QueueContainer(const QueueContainer&&) = delete;
            QueueContainer& operator=(const QueueContainer&&) = delete;
    };
}

#endif