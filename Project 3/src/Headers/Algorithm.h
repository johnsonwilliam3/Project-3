#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <unordered_map>
#include "GraphStructure.h"

class Algorithm {
    public:
    static void findBetweennessCentrality(GraphStructure& gs, GraphStructure& subgraph);
    static void Subgraph(GraphStructure& gs, std::unordered_map<std::string, int> topNodes);
    static void findFinalRank(GraphStructure& gs);
    static void findImportantNodes(GraphStructure& gs);
    static std::unordered_map<std::string, int> filterOutNodesBasedOnDegree(GraphStructure& gs, std::unordered_map<std::string, int> importantNodes);
    static std::unordered_map<std::string, int> filterOutNodesBasedOnRank(GraphStructure& gs);
    static float findMean(GraphStructure& gs);
    static float findSTD(GraphStructure& gs, int mean); 
};

#endif