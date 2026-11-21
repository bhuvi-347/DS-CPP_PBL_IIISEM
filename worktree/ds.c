#include "ds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TABLE_SIZE 101

typedef struct
{
    char name[50];
    char category[30];
    char date[20];
    char status[10];
    char contact[50];
    char imagePath[200];
} Item;

typedef struct Node
{
    Item data;
    struct Node* next;
} Node;

static Node *hashTable[TABLE_SIZE];

static unsigned int hashFunction(const char *name)
{
    unsigned int hash = 0;

    while (*name != '\0')
    {
        hash = (hash * 31u + (unsigned char)*name)
               % TABLE_SIZE;
        name++;
    }

    return hash;
}

void initList(void)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        hashTable[i] = NULL;
    }
}

void insertNode(Item newItem)
{
    unsigned int index;
    Node *newNode;

    index = hashFunction(newItem.name);

    newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    newNode->data = newItem;

    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

Node *getBucketHead(int index)
{
    if (index < 0 || index >= TABLE_SIZE)
    {
        return NULL;
    }

    return hashTable[index];
}

int getTableSize(void)
{
    return TABLE_SIZE;
}

Node *searchItem(const char *name)
{
    unsigned int index;
    Node *current;

    if (name == NULL)
    {
        return NULL;
    }

    index = hashFunction(name);
    current = hashTable[index];

    while (current != NULL)
    {
        if (strcmp(current->data.name, name) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void freeList(void)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        Node *current = hashTable[i];

        while (current != NULL)
        {
            Node *temp = current;
            current = current->next;
            free(temp);
        }

        hashTable[i] = NULL;
    }
}

void saveDatabaseC(const char *filename)
{
    FILE *file;
    int i;

    file = fopen(filename, "wb");

    if (file == NULL)
    {
        printf("Error opening file for saving!\n");
        return;
    }

    for (i = 0; i < TABLE_SIZE; i++)
    {
        Node *current = hashTable[i];

        while (current != NULL)
        {
            if (fwrite(&current->data,
                       sizeof(Item), 1, file) != 1)
            {
                printf("Error writing item!\n");
                fclose(file);
                return;
            }

            current = current->next;
        }
    }

    fclose(file);
}

void loadDatabaseC(const char *filename)
{
    FILE *file;
    Item loadedItem;

    file = fopen(filename, "rb");

    if (file == NULL)
    {
        return;
    }

    freeList();

    while (fread(&loadedItem, sizeof(Item), 1, file) == 1)
    {
        insertNode(loadedItem);
    }

    fclose(file);
}

