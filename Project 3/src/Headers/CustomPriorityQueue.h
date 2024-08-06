#ifndef PQ_H
#define PQ_H

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include "GraphStructure.h"

class CustomPriorityQueue {
    private:
    void heapifyUp(int index);
    void heapifyDown(int index);
    std::vector<std::shared_ptr<City>> heap;
    int size;

    public:
    CustomPriorityQueue();
    CustomPriorityQueue(const CustomPriorityQueue& pq);
    CustomPriorityQueue& operator=(const CustomPriorityQueue& pq);
    void insert(std::shared_ptr<City>& c);
    void printHeap();
    std::shared_ptr<City> extract();
    std::vector<std::shared_ptr<City>> requestNCities(GraphStructure& gs, const int n);
};

#endif
