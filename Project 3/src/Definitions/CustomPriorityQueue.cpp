#include "..\Headers\CustomPriorityQueue.h"

CustomPriorityQueue::CustomPriorityQueue(bool _useDegree) {
    size = 0;
    useDegree = _useDegree;
}

CustomPriorityQueue::CustomPriorityQueue(const CustomPriorityQueue& pq) {
    size = pq.size;
    for(auto node : pq.heap) {
        heap.push_back(node);
    }
}

CustomPriorityQueue& CustomPriorityQueue::operator=(const CustomPriorityQueue& pq) {
    size = pq.size;
    for(auto node : pq.heap) {
        heap.push_back(node);
    }

    return *this;
}

void CustomPriorityQueue::heapifyUp(int index) {   
    std::shared_ptr<City> c = heap[index];
    int parentIndex = (size - 1) / 2;
    if(parentIndex >= 0) {
        std::shared_ptr<City> parent = heap[parentIndex];
        if((useDegree ? (c->getDegree() > parent->getDegree()) : (c->getFinalIndex() > parent->getFinalIndex()))) {
            //Swap
            std::shared_ptr<City> temp = heap[index];
            heap[index] = heap[parentIndex];
            heap[parentIndex] = temp;
            heapifyUp(parentIndex);
        }
    }
}

void CustomPriorityQueue::heapifyDown(int index) {   
    int largestChildIndex = index;
    int leftChild = index * 2;
    int rightChild = index * 2 + 1;

    if(leftChild < size && (useDegree ? (heap[leftChild]->getDegree() > heap[largestChildIndex]->getDegree()) :
    (heap[leftChild]->getFinalIndex() > heap[largestChildIndex]->getFinalIndex()))) {
        largestChildIndex = leftChild; 
    }

    if(rightChild < size && (useDegree ? (heap[rightChild]->getDegree() > heap[largestChildIndex]->getDegree()) :
    (heap[rightChild]->getFinalIndex() > heap[largestChildIndex]->getFinalIndex()))) {
        largestChildIndex = rightChild; 
    }

    if(largestChildIndex > index) {
        //Swap
        std::shared_ptr<City> temp = heap[index];
        heap[index] = heap[largestChildIndex];
        heap[largestChildIndex] = temp;
        heapifyDown(largestChildIndex);
    }
}

void CustomPriorityQueue::insert(std::shared_ptr<City>& c) {
    heap.resize(size + 1);
    heap[size] = c;
    heapifyUp(size);
    size++; 
}

std::shared_ptr<City> CustomPriorityQueue::extract() {
    std::shared_ptr<City> extraction = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    heap.resize(size);
    return extraction;
}

void CustomPriorityQueue::printHeap() {
    for(int i = 0; i < size; i++) {
        std::cout << (useDegree ? heap[i]->getDegree() : heap[i]->getFinalIndex()) << std::endl;
    }
}

std::vector<std::shared_ptr<City>> CustomPriorityQueue::requestNCities(GraphStructure& gs, const int n){
    std::unordered_map<std::string, std::shared_ptr<City>>& cities = gs.getCities();
    
    for (auto pair : cities){
        insert(pair.second);
    }

    std::vector<std::shared_ptr<City>> cityVec(n);

    for(int i = 0; i < n; ++i){
        cityVec[i] = extract();
    }

    return cityVec;
}