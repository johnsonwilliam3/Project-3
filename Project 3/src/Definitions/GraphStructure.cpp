#include "..\Headers\GraphStructure.h"

GraphStructure::GraphStructure(const GraphStructure& gs) {
    for(int i = 0; i < gs.vertices.length(); i++) {
        Vertex* old = gs.vertices[i];
        Vertex* add = new Vertex(old->value);
        vertices.push_back(add);
    }
}

GraphStructure::operator=(const GraphStructure& gs) {
    for(int i = 0; i < gs.vertices.length(); i++) {
        Vertex* old = gs.vertices[i];
        Vertex* add = new Vertex(old->value);
        vertices.push_back(add);
    }
}

GraphStructure::~GraphStructure() {
    for(int i = 0; i < vertices.size(); i++) {
        delete vertices[i];
    }
}

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