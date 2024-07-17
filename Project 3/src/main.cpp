#include <iostream>
#include <fstream>
#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef __APPLE__
    #include <unistd.h>
#endif

#include "Headers\GraphStructure.h"
#include "Headers\Vertex.h"
#include "Headers\CustomPriorityQueue.h"
#include "Headers\Region.h"
using namespace std; 

void delay(int milliseconds) {
    #if defined(_WIN32)
        Sleep(milliseconds);
    #elif defined(__APPLE__)
        nanosleep(milliseconds * 1000);
    #endif
}

int main() {
    cout << "Welcome to the Program" << endl << endl;
    delay(1000);
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

        delay(1500);
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
        delay(3000);
        system("cls");
    }

    outfile.close();

    return 0;
}