#include "..\Headers\GraphStructure.h"

/*GraphStructure::GraphStructure(const GraphStructure& gs) {
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
*/
#include "../Headers/GraphStructure.h"
#include <queue>
#include <iostream>

/*
GraphStructure&::GraphStructure(const GraphStructure& gs) {

}

GraphStructure&::operator=(const GraphStructure& gs) {

}

GraphStructure::~GraphStructure() {
    for(int i = 0; i < vertices.size(); i++) {
        delete vertices[i];
    }
}
*/
void GraphStructure::addToMap( string key, shared_ptr<City> city)
{
   if( id_city.find(key)==id_city.end())
        id_city[key] = city; //add pop and constr once!
    else
        id_city[key]->rank.congestion_rank+=city->rank.congestion_rank;
}
GraphStructure::GraphStructure(){}


void GraphStructure::addEdge(string origin_key, string dest_key, shared_ptr<City> origin, shared_ptr<City> dest )
{ 
    adjList[origin_key].push_back(dest_key);
    adjList[dest_key].push_back(origin_key);

}
void findBetweennessCentrality(GraphStructure& graph, GraphStructure& subgraph){
    std::unordered_map<std::string, float> betweenness_centrality;
    
    for (const auto& node : subgraph.adjList) {
        betweenness_centrality[node.first] = 0.0f;
    }
    for (const auto& src : subgraph.adjList) {
        std::unordered_map<std::string, float> dist;
        std::unordered_map<std::string, std::vector<std::string>> predecessors;
        std::unordered_map<std::string, float> dependency;
        std::queue<std::string> q;
        std::vector<std::string> nodes;

        for (const auto& node : subgraph.adjList) {
            dist[node.first] = std::numeric_limits<float>::infinity();
            predecessors[node.first] = {};
            dependency[node.first] = 0.0f;
        }
        dist[src.first] = 0.0f;
        q.push(src.first);
        nodes.push_back(src.first);

        while (!q.empty()) {
            std::string current = q.front();
            q.pop();

            for (const auto& nbr : subgraph.adjList[current]) {
                if (dist[nbr] == std::numeric_limits<float>::infinity()) {
                    dist[nbr] = dist[current] + 1;
                    q.push(nbr);
                    nodes.push_back(nbr);
                }
                if (dist[nbr] == dist[current] + 1) {
                    predecessors[nbr].push_back(current);
                }
            }
        }
        while (!nodes.empty()) {
            std::string w = nodes.back();
            nodes.pop_back();

            for (const auto& v : predecessors[w]) {
                dependency[v] += (1.0f + dependency[w]) / predecessors[w].size();
            }

            if (w != src.first) {
                betweenness_centrality[w] += dependency[w];
                std::cout << w << ": " << dependency[w] << std::endl;
            }
        }
    }

    for (auto& node : subgraph.id_city) {
        if (betweenness_centrality.find(node.first) != betweenness_centrality.end()) {
            node.second->rank.centrality = betweenness_centrality[node.first];
        }
    }

    for (auto& node : graph.id_city) {
        if (betweenness_centrality.find(node.first) != betweenness_centrality.end()) {
            node.second->rank.centrality = betweenness_centrality[node.first];
        }
    }
}
 
float findMean( const GraphStructure& gs){
    int size = gs.adjList.size();
    float total=0;
    for(auto node : gs.id_city){
       auto rank= node.second->rank;
       auto current = rank.congestion_rank;  //redo: for final processing need to use in normalization for all 5 componenets of rank - centrality, congestion, constr and popluation growth
       total+= current;

    }
    return total/size;


}
float findSTD(const GraphStructure& gs,  int mean){
    int size = gs.adjList.size();
    float difference;
    float total=0;
     for(auto node : gs.id_city)
     {
        auto rank= node.second->rank;
        auto current = rank.congestion_rank; //for final processing need to use in normalization for all 3 componenets of rank, redo 
        difference= current - mean;
        total += difference*difference;
     }
     float variance = size > 0 ? total / size : 0; 
     return sqrt(variance);
}

void findFinalRank(GraphStructure& gs)
{
    findImportantNodes(gs);
    float mean =findMean(gs); //will only compute 
    float std = findSTD(gs, mean);
    priority_queue<pair<int,  string>, vector<pair< int,  string>>, less<pair<int,  string>>> pq;
    for(auto node : gs.id_city){
        auto& rank = node.second->rank; 
        int normalizedRank = rank.centrality*0.2+rank.congestion_rank*0.8; //(rank.centrality*0.2+rank.congestion_rank*0.6 + rank.construction_rank*0.1+rank.population_prediction*0.1);
        auto final = (normalizedRank - mean)/std;
        node.second->finalIndex= final*10; //updates the graph
        pq.push(make_pair((int) node.second->finalIndex, node.first));  //updates the priority queue
    }

    //for testing purpose 
    int i =0; 
    while(!pq.empty() && i<100){
        auto front = pq.top();
        cout<<++i<<" City "<<front.second<<" with a final rank of "<<front.first<<endl;
        pq.pop();
       
    }
    
}

float normalize(float rank, float mean, float std){
    return  (rank - mean)/std;
 }

//for testing 
void GraphStructure::PrintGraph(){
    
    for(auto node : adjList){
        std::cout<<std::endl<<" "<<node.first<<" ->";
        auto vec = node.second;
        for( int i =0; i <vec.size(); i++){
           if(id_city[vec[i]].get()!=nullptr) 
             std::cout<<"->"<<vec[i];
        }
    }
}
//for testing 
void GraphStructure::PrintMapAlphabetic(){
    int count=0;
    map<string, shared_ptr<City>> ordered_map(id_city.begin(), id_city.end());
    for(auto node : ordered_map){
        std::cout<<std::endl<<++count<<" "<<node.first<<" with a rank of"<<node.second->rank.congestion_rank;
   
    }
}


void Subgraph( GraphStructure& gs,  unordered_map<string, int> topNodes){
    GraphStructure subgraph;
    for(auto current : topNodes){
        auto vec = gs.adjList[current.first];
      //  std::cout<<current.second<<" with a congestion level of "<<current.first<<endl; //for testing
        subgraph.addToMap(current.first, gs.id_city[current.first] );
        for( int i =0; i<vec.size();i++){ 
            subgraph.addToMap(vec[i], gs.id_city[vec[i]] );
            subgraph.addEdge(current.first, vec[i], gs.id_city[current.first],  gs.id_city[vec[i]]);
        }
    }
    findBetweennessCentrality(gs, subgraph);
}
 unordered_map<string, int> filterOutNodesBasedOnRank( GraphStructure& gs){
    float mean =findMean(gs);
    float std = findSTD(gs,mean);
    priority_queue<pair<int,  string>, vector<pair< int,  string>>, less<pair<int,  string>>> pq;
    for(auto node : gs.id_city){
        auto& rank = node.second->rank;
        auto norm_rank = (rank.congestion_rank - mean)/std;
        pq.push(make_pair((int)norm_rank*10, node.first));
       
    }
    unordered_map<string, int> importantNodes; 
    int i=0;
    while(!pq.empty() && pq.top().first>0){
        auto front = pq.top();
        importantNodes[front.second] = front.first;
        pq.pop();  
    }
    return importantNodes;


}
void findImportantNodes(GraphStructure& gs){
   unordered_map<string, int> importantNodes  =  filterOutNodesBasedOnRank(gs);
   importantNodes = filterOutNodesBasedOnDegree(gs, importantNodes);
   Subgraph(gs, importantNodes);

}

 unordered_map<string, int> filterOutNodesBasedOnDegree( GraphStructure& gs,  unordered_map<string, int> importantNodes ){
    unordered_map<string, int> outdegree = gs.outDegree();
    priority_queue<pair<int,  string>, vector<pair< int,  string>>, less<pair<int,  string>>> pq;
    unordered_map<string, int> degree;
    for(auto node : gs.adjList){
        pq.push(make_pair(outdegree[node.first], node.first));
    }
    int n=0;
    while(!pq.empty() and n!=100){
        auto front = pq.top();
      //  std::cout<<n<<". City "<<front.second<<". Degree "<<front.first<<std::endl; //testing
        pq.pop();
        degree[front.second] = front.first;
        n++;

    }
   return degree;

}   

unordered_map<string, int>GraphStructure::outDegree(){
      unordered_map<string, int> outdegree;
    for( auto nbr : adjList){
         outdegree[nbr.first]=nbr.second.size();
    }
    return outdegree;
}