#define GRAPH_DEF
#ifndef GRAPH_DEF

#include <vector>
#include "Vertex.h"

using namespace std;

class GraphStructure {
    private:
    vector<Vertex*> vertices;

    public:
    void addEdge();
    ~GraphStructure;

}

#endif