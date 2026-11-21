#ifndef DS_H
#define DS_H
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
    char imagePath[200];
}Item;

typedef struct Node
{
    Item data;
    struct Node *next;
}Node;
void initList(void);
void insertNode(Item newItem);
Node *getBucketHead(int index);
int getTableSize(void);
Node *searchItem(const char *name);
void freeList(void);
void saveDatabaseC(const char *filename);
void loadDatabaseC(const char *filename);
#ifdef __cplusplus
}
#endif
#endif
