#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ItemNode {
    int itemId;
    char itemName[50];
    char category[30]; 
    char status[10];   
    char contactInfo[50];
    struct ItemNode* next;
};

struct ItemNode* createNode(int id, const char* name, const char* category, const char* status, const char* contact) {
    struct ItemNode* newNode = (struct ItemNode*)malloc(sizeof(struct ItemNode));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newNode->itemId = id;
    strcpy(newNode->itemName, name);
    strcpy(newNode->category, category);
    strcpy(newNode->status, status);
    strcpy(newNode->contactInfo, contact);
    newNode->next = NULL;
    
    return newNode;
}
