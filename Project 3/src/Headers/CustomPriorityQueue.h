#ifndef PQ_H
#define PQ_H

#include <memory>
#include "GraphStructure.h"

class CustomPriorityQueue {
    private:
    void heapifyUp(int index);
    void heapifyDown(int index);
    shared_ptr<City>* heap[4];
    int size;
    int capacity;

    public:
    CustomPriorityQueue();
    CustomPriorityQueue(const CustomPriorityQueue& pq);
    operator=(const CustomPriorityQueue& pq);
    ~CustomPriorityQueue();
    void insert(shared_ptr<City> c);
    void printHeap();
    shared_ptr<City>* extract(); 
};

#endif