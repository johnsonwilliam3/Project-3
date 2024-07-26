#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <queue>
#include "GraphStructure.h"

/*#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef __APPLE__
    #include <unistd.h>
#endif

#include "..\Headers\Vertex.h"
#include "..\Headers\CustomPriorityQueue.h"
#include "..\Headers\Region.h"


void delay(int milliseconds) {
    #if defined(_WIN32)
        Sleep(milliseconds);
    #elif defined(__APPLE__)
        nanosleep(milliseconds * 1000);
    #endif
}*/
void loadData(GraphStructure& gs, ifstream& file );

using namespace std;

string generateKey(string city, string county, string state){ 
    return city+" "+county+ " "+ state;
}

int main() {
    cout << "Welcome to the Program" << endl;
    ifstream file("../data/data.csv");
    if(file.is_open()) cerr<<" open";
    string line;
    map<int, vector<string>> mp;
    int counter=0;
    string header;
    int row=0;
    getline(file, header);
    GraphStructure graph;
    loadData(graph, file); //loading data into a graph
    findFinalRank(graph);


  //  delay(1000);
 //   system("cls");
    cout << "1. XXX" << endl;
    cout << "2. XXX" << endl;
    cout << "3. XXX" << endl;
    cout << "4. XXX" << endl;
    cout << "5. XXX" << endl;
    cout << "6. XXX" << endl;
    cout << "7. XXX" << endl;
    cout << "8. XXX" << endl;
    cout << "9. XXX" << endl;
    cout << "E[X]IT" << endl;
    cout << "Select a region to analyze: ";

    string resp;
    cin >> resp; 
    int numResp; 

    ofstream outfile;
    outfile.open("output\\output.txt");

    if(!(resp == "X" || resp == "x")) {
        int x = stoi(resp);
        if(outfile.is_open()) 
            outfile << resp <<  endl;

       // delay(1500);
        system("cls");
        cout << "1. Graph" << endl;
        cout << "2. Heap-Based Priority Queue" << endl;
        cout << "Select a data structure to utilize: " << endl;
        cin >> numResp;

        if(numResp == 1) {
            outfile << "Graph" << endl; 
        } 

        else {
            outfile << "Priority Queue" << endl;
        }

        outfile.close();
        system("py PythonFiles\\main.py");


    }

    else {
        cout << endl << "Exiting..." << endl;
     //   delay(3000);
        system("cls");
    }

    outfile.close();

    return 0;
}
void loadData(GraphStructure& graph, ifstream& file )
{
 string line;
 int counter;
 while(getline(file, line)){
      stringstream ss(line);  
      string cell;
      counter=-1;
      string city, county, state ,zip, start_lat, start_lng,
            north_lat, north_lng, east_lat, east_lng, west_lat,
            west_lng, south_lat, south_lng, dest_city_n, dest_state_n,
            dest_city_w, dest_state_w, dest_city_e, dest_state_e,
            dest_city_s, dest_state_s, severity, change_2020_23,
            const_cost_rank;

      while(getline(ss, cell, ',')){
        switch(counter)
        {
            case(0):city= cell; break;
            case(1):county = cell; break;
            case(2): state=cell; break;
            case(3): zip=cell; break;
            case(4): start_lat = cell;break;
            case(5):start_lng=cell; break;
            case(6): north_lat =cell; break;
            case(7): north_lng =cell; break;
            case(8):east_lat =cell; break;
            case(9):east_lng = cell; break;
            case(10): west_lat = cell;break;
            case(11):west_lng = cell; break;
            case(12):south_lat = cell; break;
            case(13): south_lng = cell; break;
            case(14):dest_city_n = cell; break;
            case(15): dest_state_n = cell; break;
            case(16):dest_city_w =cell;  break;
            case(17): dest_state_w = cell; break;
            case(18): dest_city_e = cell; break;
            case(19): dest_state_e= cell; break;
            case(20):dest_city_s = cell; break;
            case(21): dest_state_s = cell; break;
            case(22):severity= cell; break;
            case(23): change_2020_23 = cell; break;
            case(24):const_cost_rank = cell;  break;
        }
            counter++;
    } 
   

    shared_ptr<City> city1 = make_shared<City>(city, county, state, stof(start_lat), stof(start_lng),  stoi(severity), stoi(change_2020_23), 49-stoi(const_cost_rank));
    string city1_key = generateKey(city, county, state);
    graph.addToMap(city1_key, city1);
        if(dest_city_n != "NA" && city != dest_city_n && dest_city_n!=dest_city_e
             && dest_city_s!=dest_city_n  && dest_city_n!=dest_city_w){   
            string city2_key = generateKey(dest_city_n, county, state );
            shared_ptr<City> city2 = make_shared<City>(dest_city_n, county, dest_state_n, stof(north_lat), stof(north_lng),  stoi(severity), stoi(change_2020_23), 49-stoi(const_cost_rank));
            graph.addToMap(city2_key, city2);
            graph.addEdge(city1_key, city2_key, city1, city2);
        }
        if(dest_city_w != "NA" && city!=dest_city_w && dest_city_w!=dest_city_e
             && dest_city_w!=dest_city_n  && dest_city_s!=dest_city_w ){
            shared_ptr<City> city3 = make_shared<City>(dest_city_w, county, state, stof(west_lat), stof(west_lng),  stoi(severity), stoi(change_2020_23), 49-stoi(const_cost_rank));
            string city3_key = generateKey(dest_city_w, county, state);
            graph.addToMap(city3_key, city3);
            graph.addEdge(city1_key, city3_key, city1, city3);
        }
        if(dest_city_e != "NA" && city!=dest_city_e && dest_city_e!=dest_city_w
             && dest_city_e!=dest_city_n  && dest_city_e!=dest_city_s ){
            shared_ptr<City> city4 = make_shared<City>(dest_city_e, county, state, stof(east_lat), stof(east_lng), stoi(severity), stoi(change_2020_23), 49-stoi(const_cost_rank));
            string city4_key = generateKey(dest_city_e, county, state);
            graph.addToMap(city4_key, city4);
            graph.addEdge(city1_key, city4_key, city1, city4);
        }
            if(dest_city_s != "NA" && city!=dest_city_s && dest_city_s!=dest_city_e
             && dest_city_s!=dest_city_n  && dest_city_s!=dest_city_w ){ 
            shared_ptr<City> city5 = make_shared<City>(dest_city_s, county, state, stof(south_lat), stof(south_lng),  stoi(severity), stoi(change_2020_23), 49-stoi(const_cost_rank));
            string city5_key = generateKey(dest_city_s, county, state);
            graph.addToMap(city5_key, city5);
            graph.addEdge(city1_key, city5_key, city1, city5 );
        }    
        counter=-1;
    }
}
