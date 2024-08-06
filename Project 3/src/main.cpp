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
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
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
    
    loadData(graph, input); //loading data into a graph
    Algorithm::findFinalRank(graph); //Calculate rank
    graph.PrintToFile(); //Output data 

    map<int, string> stateIndex;
    stateIndex[1] = "Louisiana";
    stateIndex[2] = "Idaho";
    stateIndex[3] = "California";
    stateIndex[4] = "Florida";
    stateIndex[5] = "Texas";
    stateIndex[6] = "Maine";
    stateIndex[7] = "Nebraska";
    stateIndex[8] = "Oregon";
    stateIndex[9] = "Washington";

    system("cls");
    cout << "1. Louisiana" << endl;
    cout << "2. Idaho" << endl;
    cout << "3. Calfornia" << endl;
    cout << "4. Florida" << endl;
    cout << "5. Texas" << endl;
    cout << "6. Maine" << endl;
    cout << "7. Nebraska" << endl;
    cout << "8. Oregon" << endl;
    cout << "9. Washington" << endl;
    cout << "E[X]IT" << endl;
    cout << "Select a region to analyze: ";

    string resp;
    cin >> resp; 
    int numResp; 

    //Output path
    string pythonControlPath = "output/data.json";

    //RapidJSON initialization
    Document jsonRoot; 
    jsonRoot.SetObject();

    //Reference the Allocator, needed often as an arg
    Document::AllocatorType& a = jsonRoot.GetAllocator();

    {
        Value meta(kObjectType);
        jsonRoot.AddMember("metadata", meta, a);
    }

    //Wrap ofstream class for outputting JSON
    ofstream output(pythonControlPath);
    OStreamWrapper jsonOutput(output);
    Writer<OStreamWrapper> write(jsonOutput);

    //If not exiting
    if(!(resp == "X" || resp == "x")) {
        int x = stoi(resp);
        if(output.is_open()) {
            //Insert data into rapidjson::Value tyoe
            Value resp(stateIndex[x]);
            Value coords(kObjectType);
            assert(jsonRoot.HasMember("metadata"));
            //Region metadata
            jsonRoot["metadata"].AddMember("region", resp, a);
            double lat = 31.9;
            double lng = -93.4;
            //Add coordinates to json data
            jsonRoot["metadata"].AddMember("lat", Value(lat).Move(), a);
            jsonRoot["metadata"].AddMember("long", Value(lng).Move(), a);
            //Send to writer
            jsonRoot.Accept(write);
        }
        
        //Start Python Program
        system("py PythonFiles\\main.py");
    }

    else {
        cout << endl << "Exiting..." << endl;
        delay(1000);
        system("cls");
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
            graph.addEdge(city1_key, city5_key, city1, city5 );
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