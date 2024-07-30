#ifndef PQ_H
#define PQ_H

#include "GraphStructure.h"

class CustomPriorityQueue {
    private:
    void heapifyUp(int index);
    void heapifyDown(int index);
    City** heap[4];
    int size;
    int capacity;

    public:
    CustomPriorityQueue();
    CustomPriorityQueue(const CustomPriorityQueue& pq);
    operator=(const CustomPriorityQueue& pq);
    ~CustomPriorityQueue();
    void insert(City* c);
    void printHeap();
};

#endif