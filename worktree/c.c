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