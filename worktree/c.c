#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// C-Style Structs
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

// Global head pointer managed by C
Node* head = NULL;

void initList() {
    head = NULL;
}

void insertNode(Item newItem) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return; 

    newNode->data = newItem;
    newNode->next = head;
    head = newNode;
}

Node* getHead() {
    return head;
}

void freeList() {
    Node* current = head;
    while (current != NULL) {
        Node* nextNode = current->next;
        free(current);
        current = nextNode;
    }
    head = NULL;
}

// C-Based File Write
void saveDatabaseC(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return;

    Node* current = head;
    while (current != NULL) {
        fwrite(&(current->data), sizeof(Item), 1, file);
        current = current->next;
    }
    fclose(file);
}

// C-Based File Read
void loadDatabaseC(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return;

    Item loadedItem;
    while (fread(&loadedItem, sizeof(Item), 1, file) == 1) {
        insertNode(loadedItem);
    }
    fclose(file);
}