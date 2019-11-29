#include <stdio.h>

typedef struct DoubleList doubleList;
typedef struct LinkedList linkedList;
typedef struct Data Data;

struct Data {
    char matrix[9];
    int cell;
    char nowGamer;

};
struct DoubleList {
    struct Data* data;
    doubleList *next;
    doubleList *prev; 
};
struct LinkedList {

doubleList *head;
int size;
};
void init();
void AddList(char value[9], int cell,char nowGamer);
void DeleteFromList(int position);
int getLength();
Data* get( int pos);
void DeleteList();