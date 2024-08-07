#define RAPIDJSON_HAS_STDSTRING 1

#include <fstream>
#include <iostream>
#include <map>
#include <memory> 
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>

#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef __APPLE__
    #include <unistd.h>
#endif

#include "Headers/GraphStructure.h"
#include "Headers/Algorithm.h"
#include "Headers/CustomPriorityQueue.h"

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/ostreamwrapper.h"

using namespace rapidjson;
using namespace std; 

void loadData(GraphStructure& gs, ifstream& file);
string generateKey(string city, string county, string state);
void delay(int milliseconds);

int main() {
    cout << "Welcome to the Program" << endl;
    //Sleep 500 ms, OS dependent | Guards defined
    delay(500);
    
    ifstream input("input/data.csv"); //Filestreams open files relative to the project root folder. It's weird.
    //Test input access
    if(input.is_open()) { std::cerr << "File Open" << endl; } else { std::cerr << "Not open" << endl;}
    map<int, vector<string>> mp;
    GraphStructure graph;

    int counter = 0;
    int row = 0;
    string header;
    string line;
    getline(input, header);

    bool useCentralityAsMethod = false;
    string resp;
    cout << "Which method would you like to use to determine importance?" << endl;
    cout << "1. Betweenness Centrality" << endl;
    cout << "2. Degree Calculation" << endl; 
    cout << "All responses other than \"2\" will select Betweenness Centrality." << endl;

    cin >> resp;
    if(resp != "2") {
        useCentralityAsMethod = true;
    }

    loadData(graph, input); //loading data into a graph

    if(useCentralityAsMethod) {
        Algorithm::findFinalRank(graph); //Calculate rank
    }
    
    CustomPriorityQueue cpq(!useCentralityAsMethod);

    cout << "How many cities would you like to track?" << endl;
    int n;
    cin >> n;
    vector<shared_ptr<City>> topCities = cpq.requestNCities(graph, n);

    //Output path
    string pythonControlPath = "output/data.json";

    //RapidJSON initialization
    Document jsonRoot; 
    jsonRoot.SetObject();

    graph.PrintToFile(); //Output data for debug

    //Wrap ofstream class for outputting JSON
    ofstream output(pythonControlPath);
    OStreamWrapper jsonOutput(output);
    Writer<OStreamWrapper> write(jsonOutput);

    //Reference the Allocator, needed often as an arg
    Document::AllocatorType& a = jsonRoot.GetAllocator();

    if(output.is_open()) {
        Value topCitiesJSON(kArrayType);
        for(int x = 0; x < n; x++) {
            Value currentCity(kObjectType);
            string cName = topCities[x]->getCity();
            string countyName = topCities[x]->getCounty();
            string stateName = topCities[x]->getState();
            float latToOutput = topCities[x]->getLatitude();
            float longToOutput = topCities[x]->getLongitude();
            int congestionRank = topCities[x]->getCongestRank();
            int popRank = topCities[x]->getPopRank();
            int constrRank = topCities[x]->getConstrRank();
            float cnt = topCities[x]->getFinalIndex();
            float deg = topCities[x]->getWeightedDegree();
            {
                Value cityToValue(cName.data(), cName.size(), a);
                Value countyToValue(countyName.data(), countyName.size(), a);
                Value stateToValue(stateName.data(), stateName.size(), a);
                Value latToValue(latToOutput);
                Value longToValue(longToOutput);
                Value congestionToValue(congestionRank);
                Value popToValue(popRank);
                Value constrToValue(constrRank);

                if(useCentralityAsMethod) {
                    Value cntToValue(cnt);
                    currentCity.AddMember("weighted_centrality", cntToValue, a);
                }

                else {
                    Value degToValue(deg);
                    currentCity.AddMember("weighted_degree", degToValue, a);
                }

                currentCity.AddMember("city", cityToValue, a);
                currentCity.AddMember("county", countyToValue, a);
                currentCity.AddMember("state", stateToValue, a);
                currentCity.AddMember("latitude", latToValue, a);
                currentCity.AddMember("longitude", longToValue, a);
                currentCity.AddMember("congestion_rank", congestionToValue, a);
                currentCity.AddMember("population_rank", popToValue, a);
                currentCity.AddMember("construction_rank", constrToValue, a);
            }

            topCitiesJSON.PushBack(currentCity, a);
        }

        jsonRoot.AddMember("topCities", topCitiesJSON, a);
    }

    {
        Value meta(kObjectType);
        jsonRoot.AddMember("metadata", meta, a);
    }

    int x = stoi(resp);
    if(output.is_open()) {
        //Insert data into rapidjson::Value tyoe
        string i = "Louisiana"; //Default location will be Indiana, user can look all around the US.
        //Initially I believed the possiblities of mapping to be more limited. The user
        //does not need to pick a specific area to look at. 
        Value r(i.data(), i.size(), a);
        Value coords(kObjectType);
        Value ucam(useCentralityAsMethod);
        assert(jsonRoot.HasMember("metadata"));
        //Region metadata
        jsonRoot["metadata"].AddMember("region", r, a);
        jsonRoot["metadata"].AddMember("use_centrality", ucam, a);
        double lat = 31.9;
        double lng = -93.4;
        //Add coordinates to json data
        jsonRoot["metadata"].AddMember("lat", Value(lat).Move(), a);
        jsonRoot["metadata"].AddMember("long", Value(lng).Move(), a);
        //Send to writer
        jsonRoot.Accept(write);
    
    
        //Start Python Program
        system("py PythonFiles\\main.py");
    }

    //Close file streams
    input.close();
    output.close();

    return 0;
}

void loadData(GraphStructure& graph, ifstream& file){
    string line;
    int counter = -1;

    //Iterate through each line
    while(getline(file, line)){
        stringstream ss(line);  
        string cell;
        counter = -1;
        string city, county, state ,zip, start_lat, start_lng,
            north_lat, north_lng, east_lat, east_lng, west_lat,
            west_lng, south_lat, south_lng, dest_city_n, dest_state_n,
            dest_city_w, dest_state_w, dest_city_e, dest_state_e,
            dest_city_s, dest_state_s, severity, change_2020_23,
            const_cost_rank;

        //Iterate through each cell and set temporary variables
        while(getline(ss, cell, ',')){
            switch(counter) {
                case(0): city = cell; break;
                case(1): county = cell; break;
                case(2): state = cell; break;
                case(3): zip = cell; break;
                case(4): start_lat = cell; break;
                case(5): start_lng = cell; break;
                case(6): north_lat = cell; break;
                case(7): north_lng = cell; break;
                case(8): east_lat = cell; break;
                case(9): east_lng = cell; break;
                case(10): west_lat = cell; break;
                case(11): west_lng = cell; break;
                case(12): south_lat = cell; break;
                case(13): south_lng = cell; break;
                case(14): dest_city_n = cell; break;
                case(15): dest_state_n = cell; break;
                case(16): dest_city_w = cell; break;
                case(17): dest_state_w = cell; break;
                case(18): dest_city_e = cell; break;
                case(19): dest_state_e = cell; break;
                case(20): dest_city_s = cell; break;
                case(21): dest_state_s = cell; break;
                case(22): severity = cell; break;
                case(23): change_2020_23 = cell; break;
                case(24): const_cost_rank = cell; break;
                default: break;
            }

            //Move to next cell
            counter++;
        }

        shared_ptr<City> city1 = make_shared<City>(city, county, state, stof(start_lat), stof(start_lng), stoi(severity), stoi(change_2020_23), 49 - stoi(const_cost_rank));
        string city1_key = generateKey(city, county, state);
        graph.addToMap(city1_key, city1);

        if(dest_city_n != "NA" && city != dest_city_n && dest_city_n != dest_city_e
            && dest_city_s != dest_city_n && dest_city_n != dest_city_w) {   
            string city2_key = generateKey(dest_city_n, county, state);
            shared_ptr<City> city2 = make_shared<City>(dest_city_n, county, dest_state_n, stof(north_lat), stof(north_lng),  stoi(severity), stoi(change_2020_23), 49 - stoi(const_cost_rank));
            graph.addToMap(city2_key, city2);
            graph.addEdge(city1_key, city2_key, city1, city2);
        }

        if(dest_city_w != "NA" && city != dest_city_w && dest_city_w != dest_city_e
            && dest_city_w != dest_city_n && dest_city_s != dest_city_w) {
            shared_ptr<City> city3 = make_shared<City>(dest_city_w, county, state, stof(west_lat), stof(west_lng), stoi(severity), stoi(change_2020_23), 49 - stoi(const_cost_rank));
            string city3_key = generateKey(dest_city_w, county, state);
            graph.addToMap(city3_key, city3);
            graph.addEdge(city1_key, city3_key, city1, city3);
        }

        if(dest_city_e != "NA" && city != dest_city_e && dest_city_e != dest_city_w
            && dest_city_e != dest_city_n  && dest_city_e != dest_city_s) {
            shared_ptr<City> city4 = make_shared<City>(dest_city_e, county, state, stof(east_lat), stof(east_lng), stoi(severity), stoi(change_2020_23), 49 - stoi(const_cost_rank));
            string city4_key = generateKey(dest_city_e, county, state);
            graph.addToMap(city4_key, city4);
            graph.addEdge(city1_key, city4_key, city1, city4);
        }

        if(dest_city_s != "NA" && city != dest_city_s && dest_city_s != dest_city_e
            && dest_city_s != dest_city_n && dest_city_s != dest_city_w) { 
            shared_ptr<City> city5 = make_shared<City>(dest_city_s, county, state, stof(south_lat), stof(south_lng), stoi(severity), stoi(change_2020_23), 49 - stoi(const_cost_rank));
            string city5_key = generateKey(dest_city_s, county, state);
            graph.addToMap(city5_key, city5);
            graph.addEdge(city1_key, city5_key, city1, city5);
        }    

        counter = -1;
    }
}

string generateKey(string city, string county, string state) { 
    return city + " " + county + " " + state;
}

void delay(int milliseconds) {
    #if defined(_WIN32)
        Sleep(milliseconds);
    #elif defined(__APPLE__)
        nanosleep(milliseconds * 1000);
    #endif
}