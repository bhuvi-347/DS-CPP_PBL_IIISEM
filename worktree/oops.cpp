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