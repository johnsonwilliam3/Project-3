#ifndef GRAPH_H
#define GRAPH_H
#include <map>
#include <string>
#include <vector>

struct Rank{
    int congestion_rank;
    int population_prediction;
    int construction_rank; 
    int centrality =0;
    void update_rank(int congest_rank, int pop_pred, int constr_rank ){};
    Rank(int congestionRank, int populationPrediction, int constructionRank)
       : congestion_rank(congestionRank), population_prediction(populationPrediction), construction_rank(constructionRank) {}
};

class City{
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
        rank.congestion_rank+=congest_rank;
        rank.population_prediction +=pop_pred;
        rank.construction_rank+=constr_rank;
    }

    std::string getCity(){
        return city;
    }

    City(std::string cty, std::string County, std::string State, float Lat, float rnk, int congest_rank, float pop_pred, int constr_rank)
        : city(cty), county(County), state(State), lat(Lat), lng(rnk), rank(congest_rank,pop_pred, constr_rank) {}

    const int constrRank() {
        return rank.construction_rank;
    }

    const float popRank() {
        return rank.population_prediction;
    }

    const int congestRank() {
        return rank.congestion_rank;
    }

     void updateFinalRank(float n) {
        finalIndex += n;
    }
};

class GraphStructure {
    private:
    std::unordered_map<std::string, std::vector<string>> adjList; 
    std::unordered_map<std::string, std::shared_ptr<City>> id_city;

    public:
    GraphStructure();
    void addToMap(std::string key, std::shared_ptr<City> city); 
    void addEdge(std::string origin_key, std::string dest_key, std::shared_ptr<City> origin, std::shared_ptr<City> dest );
    void PrintGraph();
    void PrintMapAlphabetic();
    unordered_map<string, int> outDegree();
};

/*  */

#endif