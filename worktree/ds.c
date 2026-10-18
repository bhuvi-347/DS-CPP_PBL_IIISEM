#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

static Node* head = NULL;
static Node* tail = NULL;

static int sameItem(const Item* a, const Item* b)
{
    return strcmp(a->name, b->name) == 0 &&
           strcmp(a->category, b->category) == 0 &&
           strcmp(a->date, b->date) == 0 &&
           strcmp(a->status, b->status) == 0 &&
           strcmp(a->contact, b->contact) == 0;
}

void initList(void)
{
    head = NULL;
    tail = NULL;
}

int insertNode(Item newItem)
{
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) return 0;

    n->data = newItem;
    n->next = head;
    head = n;
    if (tail == NULL) tail = n;
    return 1;
}

static int appendNode(Item it)
{
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) return 0;

    n->data = it;
    n->next = NULL;
    if (tail != NULL) tail->next = n;
    else head = n;
    tail = n;
    return 1;
}

Node* getHead(void)
{
    return head;
}

Node* findNode(const Item* key)
{
    Node* cur = head;
    while (cur != NULL)
    {
        if (sameItem(&cur->data, key)) return cur;
        cur = cur->next;
    }
    return NULL;
}

int removeNode(const Item* key)
{
    Node* prev = NULL;
    Node* cur = head;
    while (cur != NULL)
    {
        if (sameItem(&cur->data, key))
        {
            if (prev != NULL) prev->next = cur->next;
            else head = cur->next;
            if (cur == tail) tail = prev;
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

void freeList(void)
{
    Node* cur = head;
    while (cur != NULL)
    {
        Node* next = cur->next;
        free(cur);
        cur = next;
    }
    head = NULL;
    tail = NULL;
}

int saveDatabaseC(const char* filename)
{
    FILE* f = fopen(filename, "wb");
    if (f == NULL) return 0;

    Node* cur = head;
    while (cur != NULL)
    {
        if (fwrite(&cur->data, sizeof(Item), 1, f) != 1)
        {
            fclose(f);
            return 0;
        }
        cur = cur->next;
    }
    return fclose(f) == 0;
}

int loadDatabaseC(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    if (f == NULL) return -1;

    freeList();

    Item it;
    int count = 0;
    while (fread(&it, sizeof(Item), 1, f) == 1)
    {
        it.name[sizeof it.name - 1] = '\0';
        it.category[sizeof it.category - 1] = '\0';
        it.date[sizeof it.date - 1] = '\0';
        it.status[sizeof it.status - 1] = '\0';
        it.contact[sizeof it.contact - 1] = '\0';

        if (!appendNode(it)) break;
        count++;
    }
    fclose(f);
    return count;
}

typedef struct StackNode
{
    Action action;
    struct StackNode* below;
} StackNode;

static StackNode* top = NULL;
static int stackCount = 0;

int pushAction(ActionType type, const Item* item)
{
    StackNode* s = (StackNode*)malloc(sizeof(StackNode));
    if (s == NULL) return 0;

    s->action.type = type;
    s->action.item = *item;
    s->below = top;
    top = s;
    stackCount++;
    return 1;
}

int popAction(Action* out)
{
    if (top == NULL) return 0;

    StackNode* s = top;
    *out = s->action;
    top = s->below;
    free(s);
    stackCount--;
    return 1;
}

int stackSize(void)
{
    return stackCount;
}

void clearStack(void)
{
    Action dummy;
    while (popAction(&dummy)) { }
}
