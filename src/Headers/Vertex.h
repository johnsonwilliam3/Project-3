#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

class Vertex {
    public:
    Vertex(int _value);
    int value;

    private:
    std::vector<std::pair<Vertex*, double>> adjacencyList;
};

#endif