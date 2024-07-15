#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std; 

int main() {
    cout << "Welcome to the Program" << endl << endl;
    Sleep(1000);
    cout << "1. XXX" << endl;
    cout << "2. XXX" << endl;
    cout << "3. XXX" << endl;
    cout << "4. XXX" << endl;
    cout << "5. XXX" << endl;
    cout << "6. XXX" << endl;
    cout << "7. XXX" << endl;
    cout << "8. XXX" << endl;
    cout << "9. XXX" << endl;
    cout << "e<X>it" << endl;
    cout << endl << "Select a region to analyze: ";

    string resp;
    cin >> resp; 

    ofstream outfile;
    outfile.open("output\\output.txt");

    if(!(resp == "X" || resp == "x")) {
        int x = stoi(resp);
        if(outfile.is_open()
            outfile << resp << endl;
    }

    else {
        cout << endl << "Exiting..." << endl;
        Sleep(3000);
    }

    outfile.close();

    return 0;
}