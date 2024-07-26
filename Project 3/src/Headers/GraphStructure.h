#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <map>
#include <string>
#include <vector>

#include "Vertex.h"

using namespace std;
/*
class GraphStructure {
    private:
    vector<Vertex*> vertices;

    public:
    GraphStructure(const GraphStructure& gs);
    operator=(const GraphStructure& gs);
    ~GraphStructure();
    void addEdge();
};
*/
struct Rank{
    int congestion_rank;
    int population_prediction;
    int construction_rank; 
    int centrality =0;
    void update_rank(int congest_rank, int pop_pred, int constr_rank ){};
    Rank(int congestionRank, int populationPrediction, int constructionRank)
       : congestion_rank(congestionRank), population_prediction(populationPrediction), construction_rank(constructionRank){}

};

class City{
public:
    string city;
    string county;
    string state;
    float lat;
    float lng;
    Rank rank;
    float finalIndex;

   
public:
    void updateRank(int congest_rank, float pop_pred, int constr_rank){
        rank.congestion_rank+=congest_rank;
        rank.population_prediction +=pop_pred;
        rank.construction_rank+=constr_rank;
    }
 string getCity(){
        return city;
    }
    City( string cty, string County, string State, float Lat, float rnk, int congest_rank, float pop_pred, int constr_rank)
        : city(cty), county(County), state(State), lat(Lat), lng(rnk), rank(congest_rank,pop_pred, constr_rank){}
    const int constrRank(){
        return rank.construction_rank;
    }
    const float popRank(){
        return rank.population_prediction;
    }
    const int congestRank(){
        return rank.congestion_rank;
    }
     void updateFinalRank(float n){
        finalIndex+=n;
    }



};
class GraphStructure {
public:
    unordered_map<string, vector<string>> adjList; 
    unordered_map<string, shared_ptr<City>> id_city;
    public:
    GraphStructure();
    //GraphStructure(const GraphStructure& gs); 
    //GraphStructure& operator=(const GraphStructure& gs );
    GraphStructure& operator=(const GraphStructure& gs); 
    void addToMap(string key, shared_ptr<City> city); 
    void addEdge(string origin_key, string dest_key, shared_ptr<City> origin, shared_ptr<City> dest );
    void PrintGraph();
    void PrintMapAlphabetic();
    unordered_map<string, int>  outDegree();

};

//need to encapsulate the following methods into another algortithm class
void findBetweennessCentrality(GraphStructure& graph,  GraphStructure& subgraph); //finds betweenness centrality of each node
void Subgraph( GraphStructure& gs,  unordered_map<string, int> topNodes); //creates a subgraph with n nodes from important nodes 
void findImportantNodes(GraphStructure& gs); //finds the most important cities for betweenness centrality
unordered_map<string, int> filterOutNodesBasedOnDegree( GraphStructure& gs,  unordered_map<string, int> importantNodes ); //keeps n nodes with highest degree to proccess in betweenness centrality 
void findFinalRank(GraphStructure& gs); //computes final rank, updates the graph and priority queue that can be used later
unordered_map<string, int> filterOutNodesBasedOnRank( GraphStructure& gs); // based on z-score normalization filters out data below the mean






#endif