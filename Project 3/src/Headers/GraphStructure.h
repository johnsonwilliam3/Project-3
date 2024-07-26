#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <map>
#include <string>
#include <vector>
#include "Vertex.h"

using namespace std;

class GraphStructure {
    private:
    vector<Vertex*> vertices;

    public:
    GraphStructure(const GraphStructure& gs);
    operator=(const GraphStructure& gs);
    ~GraphStructure();
    void addEdge();
};

#endif