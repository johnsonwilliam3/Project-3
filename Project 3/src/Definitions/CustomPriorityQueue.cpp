#include "..\Headers\CustomPriorityQueue.h"

CustomPriorityQueue::CustomPriorityQueue() {
    size = 0;
    capacity = 4;
}

CustomPriorityQueue::CustomPriorityQueue(const CustomPriorityQueue& pq) {
    size = pq.size;
    capacity = pq.capacity;
    for(int i = 0; i < size; i++) {
        heap[i] = pq.heap[i];
    }
}

CustomPriorityQueue::operator=(const CustomPriorityQueue& pq) {
    size = pq.size;
    capacity = pq.capacity;
    for(int i = 0; i < size; i++) {
        heap[i] = pq.heap[i];
    }
}

CustomPriorityQueue::~CustomPriorityQueue() {
    delete[] heap;
}

void CustomPriorityQueue::heapifyUp(int index) {   
    Region* r = heap[index];
    int parentIndex = (size - 1) / 2;
    if(parentIndex >= 0) {
        City* parent = heap[parentIndex];
        if(r->getIdealIndex() > parent->getFinalIndex()) {
            //Swap
            City* temp = heap[index];
            heap[index] = heap[parentIndex];
            heap[parentIndex] = temp;
            heapify(parentIndex);
        }
    }


void CustomPriorityQueue::heapifyDown(int index) {   
    Region* r = heap[index];
    int largestChildIndex = index;
    int leftChild = index * 2;
    int rightChild = index * 2 + 1;

    if(leftChild < size && heap[leftChild]->idealIndex > heap[largestChildIndex]->idealIndex) {
        largestChildIndex = leftChild; 
    }

    if(rightChild < size && heap[rightChild]->idealIndex > heap[largestChildIndex]->idealIndex) {
        largestChildIndex = rightChild; 
    }

    if(largestChildIndex > index) {
            //Swap
            shared_ptr<City> temp = heap[index];
            heap[index] = heap[largestChildIndex];
            heap[largestChildIndex] = temp;
            heapifyDown(largestChildIndex);
        }
    }
}

void CustomPriorityQueue::insert(shared_ptr<City> c) {
    heap[size] = r;
    heapifyUp(size);
    size++; 
    if(size == capacity) {
        shared_ptr<City>* copy = new shared_ptr<City>[capacity * 2];
        for(int i = 0; i < size; i++) {
            copy[i] = heap[i];
        }

        delete[] heap;
        heap = copy;
    }

    else if(size <= capacity / 2 && capacity > 4) {
        shared_ptr<City>* copy = new shared_ptr<City>[capacity / 2];
        for(int i = 0; i < size; i++) {
            copy[i] = heap[i];
        }

        delete[] heap;
        heap = copy;
    }
}

CustomPriorityQueue::shared_ptr<City> extract() {
    Region* extraction = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);

    if(size <= capacity / 2 && capacity > 4) {
        shared_ptr<City>* copy = new shared_ptr<City>[capacity / 2];
        for(int i = 0; i < size; i++) {
            copy[i] = heap[i];
        }

        delete[] heap;
        heap = copy;
    }
}

void CustomPriorityQueue::printHeap() {
    for(int i = 0; i < size; i++) {
        std::cout << heap[i]->getFinalIndex()) << std::endl;
    }
}