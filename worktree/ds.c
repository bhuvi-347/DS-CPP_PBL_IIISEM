#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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