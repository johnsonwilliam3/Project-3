#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cmath>
#include <map>
#include <string>
#include <vector>
#include "GraphStructure.h"

void findBetweennessCentrality(GraphStructure& graph, GraphStructure& subgraph); //finds betweenness centrality of each node
void Subgraph(GraphStructure& gs, std::unordered_map<std::string, int> topNodes); //creates a subgraph with n nodes from important nodes 
void findImportantNodes(GraphStructure& gs); //finds the most important cities for betweenness centrality
std::unordered_map<std::string, int> filterOutNodesBasedOnDegree(GraphStructure& gs,  std::unordered_map<string, int> importantNodes); //keeps n nodes with highest degree to proccess in betweenness centrality 
void findFinalRank(GraphStructure& gs); //computes final rank, updates the graph and priority queue that can be used later
std::unordered_map<std::string, int> filterOutNodesBasedOnRank(GraphStructure& gs); // based on z-score normalization filters out data below the mean

float findMean(const GraphStructure& gs);
float findSTD(const GraphStructure& gs, int mean);
float normalize(float rank, float mean, float std);

namespace algorithm {
    void findBetweennessCentrality(GraphStructure& graph, GraphStructure& subgraph) {
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

    void Subgraph(GraphStructure& gs,  std::unordered_map<std::string, int> topNodes){
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

    void findImportantNodes(GraphStructure& gs) {
        std::unordered_map<std::string, int> importantNodes = filterOutNodesBasedOnRank(gs);
        importantNodes = filterOutNodesBasedOnDegree(gs, importantNodes);
        Subgraph(gs, importantNodes);
    }

    std::unordered_map<std::string, int> filterOutNodesBasedOnDegree(GraphStructure& gs, std::unordered_map<std::string, int> importantNodes) {
        std::unordered_map<std::string, int> outdegree = gs.outDegree();
        std::priority_queue<std::pair<int, std::string>, std::vector<std::pair< int, std::string>>, std::less<std::pair<int, std::string>>> pq;
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

    void findFinalRank(GraphStructure& gs) {
        findImportantNodes(gs);
        float mean = findMean(gs); //will only compute 
        float std = findSTD(gs, mean);
        std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::less<std::pair<int, std::string>>> pq;
        
        for(auto node : gs.id_city ){
            auto& rank = node.second->rank; 
            int normalizedRank = rank.centrality * 0.2 + rank.congestion_rank * 0.8; //(rank.centrality*0.2+rank.congestion_rank*0.6 + rank.construction_rank*0.1+rank.population_prediction*0.1);
            auto final = (normalizedRank - mean) / std;
            node.second->finalIndex = final * 10; //updates the graph
            pq.push(std::make_pair((int) node.second->finalIndex, node.first));  //updates the priority queue
        }

        //for testing purpose 
        int i = 0;

        while(!pq.empty() && i <1 00) {
            auto front = pq.top();
            std::cout << ++i << " City " << front.second << " with a final rank of " << front.first << std::endl;
            pq.pop();
        }
    }

    std::unordered_map<std::string, int> filterOutNodesBasedOnRank(GraphStructure& gs){
        float mean = findMean(gs);
        float std = findSTD(gs,mean);
        std::priority_queue<std::pair<int, std::string>, std::vector<pair::<int, std::string>>, std::less<std::pair<int, std::string>>> pq;

        for(auto node : gs.id_city){
            auto& rank = node.second->rank;
            auto norm_rank = (rank.congestion_rank - mean) / std;
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

    float findMean( const GraphStructure& gs) {
        int size = gs.adjList.size();
        float total = 0;

        for(auto node : gs.id_city) {
            auto rank= node.second->rank;
            auto current = rank.congestion_rank;  //redo: for final processing need to use in normalization for all 5 componenets of rank - centrality, congestion, constr and popluation growth
            total += current;
        }

        return total / size;
    }

    float findSTD(const GraphStructure& gs, int mean) { //Standard Deviation
        int size = gs.adjList.size();
        float difference;
        float total = 0;

        for(auto node : gs.id_city) {
            auto rank= node.second->rank;
            auto current = rank.congestion_rank; //for final processing need to use in normalization for all 3 componenets of rank, redo 
            difference= current - mean;
            total += difference * difference; //difference^2
        }

        float variance = size > 0 ? total / size : 0; 
        return sqrtf(variance);
    }

    float normalize(float rank, float mean, float std) {
        return  (rank - mean) / std;
    }
}

#endif