#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "Containers/LinkedList.h"

namespace Storage
{
    template<class T>
    class QueueContainer
    {
        public:

            QueueContainer(){}
            ~QueueContainer(){}

            void enqueue(std::shared_ptr<T> item){mContainer.insertAtEnd(item);}
            T dequeue(){return mContainer.popFront();}
            void search(unsigned long pKey) { return mContainer.search(pKey); }
            void remove(unsigned long pKey){}
            void clear(){mContainer.clear();}
            void print(){mContainer.printLinkedList();}

        private:
            Storage::SingleLinkedList<T> mContainer;
            QueueContainer(const QueueContainer&) = delete;
            QueueContainer& operator=(const QueueContainer&) = delete;
            QueueContainer(const QueueContainer&&) = delete;
            QueueContainer& operator=(const QueueContainer&&) = delete;
    };
}

#endif