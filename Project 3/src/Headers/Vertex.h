#define VERTEX_DEF
#ifndef VERTEX_DEF

class Vertex {
    public:
    Vertex(int _value);
    int value;

    private:
    vector<pair<Vertex*, double>> adjacencyList;
}

#endif