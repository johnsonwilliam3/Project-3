#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Rank {
    int congestion_rank = 0;
    int population_prediction = 0;
    int construction_rank = 0; 
    int centrality = 0;
    void update_rank(int congest_rank, int pop_pred, int constr_rank ){};
    Rank(int congestionRank, int populationPrediction, int constructionRank)
       : congestion_rank(congestionRank), population_prediction(populationPrediction), construction_rank(constructionRank) {}
};

class City {
    private:
    std::string city;
    std::string county;
    std::string state;
    float lat;
    float lng;
    Rank rank;
    float finalIndex;
   
    public:
    void updateRank(int congest_rank, float pop_pred, int constr_rank){
        rank.congestion_rank += congest_rank;
        rank.population_prediction += pop_pred;
        rank.construction_rank += constr_rank;
    }

    City(std::string cty, std::string County, std::string State, float Lat, float rnk, int congest_rank, float pop_pred, int constr_rank)
        : city(cty), county(County), state(State), lat(Lat), lng(rnk), rank(congest_rank,pop_pred, constr_rank) {}

    std::string getCity(){
        return city;
    }

    int getConstrRank() {
        return rank.construction_rank; 
    }

    void setConstrRank(int _rank) {
        rank.construction_rank = _rank;
    }

    int getPopRank() {
        return rank.population_prediction;
    }

    void getPopRank(int _rank) {
        rank.population_prediction = _rank;
    }

    int getCongestRank() {
        return rank.congestion_rank;
    }
    
    void setCongestRank(int _rank) {
        rank.congestion_rank = _rank;
    }

    int getCentrality() {
        return rank.centrality;; 
    }

    void setCentrality(int _centrality) {
        rank.centrality = _centrality;
    }

    float getFinalIndex() {
        return finalIndex;
    }

    void setFinalRank(float _rank) {
        finalIndex = _rank;
    }
};

class GraphStructure {
    private:
    std::unordered_map<std::string, std::vector<std::string>> adjList; 
    std::unordered_map<std::string, std::shared_ptr<City>> id_city;
    static void findBetweennessCentrality(GraphStructure& gs, GraphStructure& subgraph);
    static void Subgraph(GraphStructure& gs, std::unordered_map<std::string, int> topNodes);
    static void findImportantNodes(GraphStructure& gs);
    static std::unordered_map<std::string, int> filterOutNodesBasedOnDegree(GraphStructure& gs, std::unordered_map<std::string, int> importantNodes);
    static std::unordered_map<std::string, int> filterOutNodesBasedOnRank(GraphStructure& gs);
    static float findMean(GraphStructure& gs);
    static float findSTD(GraphStructure& gs, int mean);

    public:
    GraphStructure();
    void addToMap(std::string key, std::shared_ptr<City> city); 
    void addEdge(std::string origin_key, std::string dest_key, std::shared_ptr<City> origin, std::shared_ptr<City> dest);
    void PrintGraph();
    void PrintMapAlphabetic();
    void PrintToFile();
    std::unordered_map<std::string, int> outDegree();
    std::unordered_map<std::string, std::shared_ptr<City>> getCities();
    const int edgeCount();
    friend class Algorithm;
};

#endif
