#ifndef ITEM_H
#define ITEM_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    char name[50];
    char category[30];
    char date[20];
    char status[10];
    char contact[50];
} Item;

typedef struct Node
{
    Item data;
    struct Node* next;
} Node;

typedef enum
{
    ACTION_ADD     = 1,
    ACTION_RECOVER = 2
} ActionType;

typedef struct
{
    ActionType type;
    Item item;
} Action;

void  initList(void);
int   insertNode(Item newItem);
Node* getHead(void);
Node* findNode(const Item* key);
int   removeNode(const Item* key);
void  freeList(void);
int   saveDatabaseC(const char* filename);
int   loadDatabaseC(const char* filename);

int   pushAction(ActionType type, const Item* item);
int   popAction(Action* out);
int   stackSize(void);
void  clearStack(void);

#ifdef __cplusplus
}
#endif
#endif
