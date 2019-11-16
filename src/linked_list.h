#include <stdio.h>

typedef struct DoubleList doubleList;
typedef struct LinkedList linkedList;

struct DoubleList {
    int data;
    doubleList *next;
    doubleList *prev; 
};
struct LinkedList {

doubleList *head;
int size;
};
void init(linkedList l);
void AddList(linkedList l, int value);
int DeleteList(linkedList l,int position);
int getLength(linkedList l);
int get(linkedList l, int pos);