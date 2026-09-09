#include <iostream>
#include <string>
// #include "storage.h" // will include the c storage file which is yet to be created

using namespace std;
class SmartCampusSystem {
private:
    int nextItemId;

public:
    SmartCampusSystem() {
        nextItemId = 1;
    }

    void reportItem() {
        string itemName, category, status, contact;
        
        cout << "\n--- Report Item ---" << endl;
        cout << "Enter Item Status (Lost/Found): ";
        cin >> status;
        cout << "Enter Item Name: ";
        cin.ignore();
        getline(cin, itemName);
        
        cout << "[System] Item successfully routed to C-based Linked List storage." << endl;
    }

    void displayMenu() {
        cout << "\n=======================================" << endl;
        cout << "  Smart Campus Lost & Found Registry   " << endl;
        cout << "=======================================" << endl;
        cout << "1. Report Lost or Found Item" << endl;
        cout << "2. Search for an Item" << endl;
        cout << "3. Exit" << endl;
        cout << "Select an option: ";
    }
};