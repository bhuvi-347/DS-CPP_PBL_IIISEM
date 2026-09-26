#include <iostream>
#include <string>
#include <cstring>

using namespace std;

extern "C" {
    typedef struct {
        char name[50];
        char category[30];
        char date[20];
        char status[10];
        char contact[50];
    } Item;

    typedef struct Node {
        Item data;
        struct Node* next;
    } Node;

    void initList();
    void insertNode(Item newItem);
    Node* getHead();
    void freeList();
    void saveDatabaseC(const char* filename);
    void loadDatabaseC(const char* filename);
}

class LostAndFoundSystem {
private:
    const char* dbFile = "campus_items.dat";

    void checkMatch(Item newlyReported) {
        Node* temp = getHead();
        bool matchFound = false;
        
        cout << "\n--- Smart Matching System ---" << endl;
        while (temp != nullptr) {
            if (strcmp(temp->data.status, newlyReported.status) != 0) {
                if (strstr(temp->data.name, newlyReported.name) != nullptr || 
                    strstr(temp->data.category, newlyReported.category) != nullptr) {
                    
                    cout << "Potential Match Found!" << endl;
                    cout << "Item: " << temp->data.name << " (" << temp->data.category << ")" << endl;
                    cout << "Reported as " << temp->data.status << " on: " << temp->data.date << endl;
                    cout << "Contact: " << temp->data.contact << endl;
                    matchFound = true;
                }
            }
            temp = temp->next;
        }
        if (!matchFound) cout << "No immediate matches found in the system." << endl;
        cout << "-----------------------------\n" << endl;
    }

public:
    LostAndFoundSystem() {
        initList(); 
        loadDatabaseC(dbFile); 
    }

    ~LostAndFoundSystem() {
        saveDatabaseC(dbFile); 
        freeList(); 
    }

    void reportItem(const char* status) {
        Item newItem;
        strcpy(newItem.status, status);
        
        cin.ignore();
        cout << "Enter Item Name (e.g., Earbuds, ID Card): ";
        cin.getline(newItem.name, 50);
        
        cout << "Enter Category (Electronics, IDs, Books, etc.): ";
        cin.getline(newItem.category, 30);
        
        cout << "Enter Date (DD/MM/YYYY): ";
        cin.getline(newItem.date, 20);
        
        cout << "Enter Your Contact Info (Phone/Email): ";
        cin.getline(newItem.contact, 50);

        insertNode(newItem);
        cout << "\nItem successfully registered as " << status << "!" << endl;
        
        checkMatch(newItem);
    }

    void searchItem() {
        char searchKeyword[50];
        cin.ignore();
        cout << "Enter keyword to search (Name or Category): ";
        cin.getline(searchKeyword, 50);

        Node* temp = getHead();
        bool found = false;
        
        cout << "\n--- Search Results ---" << endl;
        while (temp != nullptr) {
            if (strstr(temp->data.name, searchKeyword) != nullptr || 
                strstr(temp->data.category, searchKeyword) != nullptr) {
                
                cout << "[" << temp->data.status << "] " 
                     << temp->data.name << " | Category: " << temp->data.category 
                     << " | Date: " << temp->data.date 
                     << " | Contact: " << temp->data.contact << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) cout << "No items match your search." << endl;
        cout << "----------------------\n" << endl;
    }

    void displayAllItems() {
        Node* temp = getHead();
        if (temp == nullptr) {
            cout << "\nThe database is currently empty.\n" << endl;
            return;
        }

        cout << "\n--- All Registered Items ---" << endl;
        while (temp != nullptr) {
            cout << "[" << temp->data.status << "] " 
                 << temp->data.name << " (" << temp->data.category << ") - " 
                 << temp->data.date << endl;
            temp = temp->next;
        }
        cout << "----------------------------\n" << endl;
    }
};