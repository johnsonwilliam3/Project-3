#include <iostream>
#include <fstream>
#include <windows.h>
#include "Headers\GraphStructure.h"
#include "Headers\Vertex.h"
#include "Headers\CustomPriorityQueue.h"
#include "Headers\Region.h"
using namespace std; 

int main() {
    cout << "Welcome to the Program" << endl << endl;
    Sleep(1000);
    system("cls");
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

        Sleep(1500);
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
        Sleep(3000);
        system("cls");
    }

    outfile.close();

    return 0;
}