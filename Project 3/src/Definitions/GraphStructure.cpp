#include "..\Headers\GraphStructure.h"

GraphStructure::addEdge(int x, int y) {
    Vertex* xV = nullptr;
    Vertex* yV = nullptr;

    for(int i = 0; i < vertices.size(); i++) {
        if (vertices[i].second == x) {
            xV = vertices[i].first;
        }

        if (vertices[i].second == y)
        {
            yV = vertices[i].first;
        }

    }

    if(!xV) {
        xV = new Vertex(x);
        vertices.push_back(xV);
    }

    if(!yV) {
        yV = new Vertex(y);
        vertices.push_back(yV);
    }

    std::pair<Vertex*, double> edge(yV, 0.0);
    xV.adjacencyList.push_back(edge);
    
}

GraphStructure::~GraphStructure() {
    for(int i = 0; i < vertices.size(); i++) {
        delete vertices[i];
    }
}