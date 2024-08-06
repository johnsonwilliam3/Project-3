#include <cmath>
#include <iostream>
#include <memory>
#include <queue>
#include "../Headers/Algorithm.h"

void Algorithm::findBetweennessCentrality(GraphStructure& gs, GraphStructure& subgraph) {
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
                // This line is used for debugging
                //std::cout << w << ": " << dependency[w] << std::endl;
            }
        }
    }

    for (auto& node : subgraph.id_city) {
        if (betweenness_centrality.find(node.first) != betweenness_centrality.end()) {
            node.second->setCentrality(betweenness_centrality[node.first]);
        }
    }

    for (auto& node : gs.id_city) {
        if (betweenness_centrality.find(node.first) != betweenness_centrality.end()) {
            node.second->setCentrality(betweenness_centrality[node.first]);
        }
    }
}

void Algorithm::Subgraph(GraphStructure& gs, std::unordered_map<std::string, int> topNodes) {
    GraphStructure subgraph;

    for(auto current : topNodes){
        auto vec = gs.adjList[current.first];
        //std::cout<<current.second<<" with a congestion level of "<<current.first<<endl; //for testing
        subgraph.addToMap(current.first, gs.id_city[current.first]);

        for( int i = 0; i < vec.size(); i++){ 
            subgraph.addToMap(vec[i], gs.id_city[vec[i]]);
            subgraph.addEdge(current.first, vec[i], gs.id_city[current.first], gs.id_city[vec[i]]);
        }
    }

    findBetweennessCentrality(gs, subgraph);
}

void Algorithm::findImportantNodes(GraphStructure& gs) {
    std::unordered_map<std::string, int> importantNodes = filterOutNodesBasedOnRank(gs);
    importantNodes = filterOutNodesBasedOnDegree(gs, importantNodes);
    Algorithm::Subgraph(gs, importantNodes);
}

void Algorithm::findFinalRank(GraphStructure& gs) {
    findImportantNodes(gs);
    const float MEAN = Algorithm::findMean(gs); //will only compute 
    const float STD = Algorithm::findSTD(gs, MEAN);
    // This line is disabled as it is only required for debugging and is unnecessary for the functions intended purpose
    // std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::less<std::pair<int, std::string>>> pq;
    std::unordered_map<std::string, std::shared_ptr<City>> cities = gs.getCities();
    for(auto node : cities){
        int normalizedRank = node.second->getCentrality() * 0.2 + node.second->getCongestRank() * 0.8; //(rank.centrality*0.2+rank.congestion_rank*0.6 + rank.construction_rank*0.1+rank.population_prediction*0.1);
        auto final = (normalizedRank - MEAN) / STD;
        node.second->setFinalRank(final * 10); //updates the graph
        // pq.push(std::make_pair((int) node.second->getFinalIndex(), node.first));  //updates the priority queue
    }

    /*
    //for testing purpose 
    int i = 0;

    while(!pq.empty() && i < 100) {
        auto front = pq.top();
        //std::cout << ++i << . City " << front.second << " with a final rank of " << front.first << std::endl;
        pq.pop();
    }
    */
}


std::unordered_map<std::string, int> Algorithm::filterOutNodesBasedOnDegree(GraphStructure& gs, std::unordered_map<std::string, int> importantNodes) {
    std::unordered_map<std::string, int> outdegree = gs.outDegree();
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::less<std::pair<int, std::string>>> pq;
    std::unordered_map<std::string, int> degree;

    for(auto node : gs.adjList){
        pq.push(std::make_pair(outdegree[node.first], node.first));
    }

    int n = 0;

    while(!pq.empty() && n != 100) {
        auto front = pq.top();
        //std::cout << n << ". City " << front.second << ". Degree " << front.first << std::endl; //testing
        pq.pop();
        degree[front.second] = front.first;
        n++;
    }
    
    return degree;
}   

std::unordered_map<std::string, int> Algorithm::filterOutNodesBasedOnRank(GraphStructure& gs) {
    const float MEAN = findMean(gs);
    const float STD = findSTD(gs, MEAN);
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::less<std::pair<int, std::string>>> pq;

    std::unordered_map<std::string, std::shared_ptr<City>> cities = gs.getCities();
    for(auto node : cities){
        float norm_rank = (node.second->getCongestRank() - MEAN) / STD;
        pq.push(std::make_pair((int) norm_rank * 10, node.first));
    }

    std::unordered_map<std::string, int> importantNodes; 
    int i = 0;

    while(!pq.empty() && pq.top().first > 0) {
        auto front = pq.top();
        importantNodes[front.second] = front.first;
        pq.pop();  
    }

    return importantNodes;
}

float Algorithm::findMean(GraphStructure& gs) {
    int size = gs.edgeCount();
    float total = 0;

    std::unordered_map<std::string, std::shared_ptr<City>> cities = gs.getCities();
    for(auto node : cities) {
        // This is the new implementation to be in accordance with the findFinalRank function
        auto current = node.second->getCentrality() * 0.15 + node.second->getCongestRank() * 0.4 + node.second->getPopRank() * 0.25 + node.second->getConstrRank() * 0.2;  
        total += current;
    }

    return total / size;
}

float Algorithm::findSTD(GraphStructure& gs, int mean) { //Standard Deviation
    int size = gs.edgeCount();
    float difference;
    float total = 0;

    std::unordered_map<std::string, std::shared_ptr<City>> cities = gs.getCities();
    for(auto node : cities) {
        // This has also been changed to be in accordance with the new findFinalRank function
         auto current = node.second->getCentrality() * 0.15 + node.second->getCongestRank() * 0.4 + node.second->getPopRank() * 0.25 + node.second->getConstrRank() * 0.2;
        total += difference * difference; //difference^2
    }

    float variance = size > 0 ? total / size : 0.0; 
    return sqrtf(variance);
}
