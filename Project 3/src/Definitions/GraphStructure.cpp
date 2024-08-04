#include <cmath>
#include <fstream>  
#include <iostream>
#include <memory>
#include <queue>
#include "../Headers/GraphStructure.h"

GraphStructure::GraphStructure() {}

void GraphStructure::addToMap(std::string key, std::shared_ptr<City> city) {
    if(id_city.find(key) == id_city.end()) {
        id_city[key] = city; //add pop and constr once!
    }

    else {
        id_city[key]->setCongestRank(id_city[key]->getCongestRank() + city->getCongestRank());
    }
}

void GraphStructure::addEdge(std::string origin_key, std::string dest_key, std::shared_ptr<City> origin, std::shared_ptr<City> dest) { 
    adjList[origin_key].push_back(dest_key);
    adjList[dest_key].push_back(origin_key);
}

//For Testing 
void GraphStructure::PrintGraph(){
    for(auto node : adjList){
        std::cout << std::endl << " " << node.first;
        auto vec = node.second;

        for( int i = 0; i < vec.size(); i++) {
            if(id_city[vec[i]].get() != nullptr) {
                std::cout << " -> " << vec[i];
            }
        }
    }
}

//For Testing 
void GraphStructure::PrintMapAlphabetic(){
    int count = 0;
    std::map<std::string, std::shared_ptr<City>> ordered_map(id_city.begin(), id_city.end());

    for(auto node : ordered_map){
        std::cout << std::endl << ++count << " " << node.first << " with a rank of " << node.second->getCongestRank();
    }
}

//For Testing 
void GraphStructure::PrintToFile(){
    std::ofstream output("output/graph.txt");

    for(auto node : adjList){
        output << std::endl << " " << node.first;
        auto vec = node.second;

        for( int i = 0; i < vec.size(); i++) {
            if(id_city[vec[i]].get() != nullptr) {
                output << " -> " << vec[i];
            }
        }
    }

    output.close();
}

std::unordered_map<std::string, int> GraphStructure::outDegree(){
    std::unordered_map<std::string, int> outdegree;

    for(auto nbr : adjList){
         outdegree[nbr.first] = nbr.second.size();
    }

    return outdegree;
}

const int GraphStructure::edgeCount() {
    return adjList.size();
}
std::unordered_map<std::string, std::shared_ptr<City>>& GraphStructure::getCities(){
    return id_city;
}
