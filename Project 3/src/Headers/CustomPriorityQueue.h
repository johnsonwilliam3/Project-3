#ifndef PQ_H
#define PQ_H

#include "Region.h"

class CustomPriorityQueue {
    private:
    void heapifyUp(int index);
    void heapifyDown(int index);
    Region** heap[4];
    int size;
    int capacity;

    public:
    CustomPriorityQueue();
    CustomPriorityQueue(const CustomPriorityQueue& pq);
    operator=(const CustomPriorityQueue& pq);
    ~CustomPriorityQueue();
    void insert(Region* r);
    Region* extract();
};

#endif