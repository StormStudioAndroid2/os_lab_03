#include "linked_list.h"
#include <stdlib.h>
doubleList* head;
int size;
void AddList(char value[9],int cell,char nowGamer) {
    Data* dd = (Data*)malloc(sizeof(Data));
 
    doubleList* node = (doubleList*) malloc(sizeof(doubleList)); 
    for (int i = 0; i<9; ++i) {
        dd->matrix[i] = value[i];
    }
    dd->cell = cell;
    dd->nowGamer = nowGamer;
   
    node->data = dd;
    if (head==NULL) {
        node->next=NULL;
        node->prev=NULL; 
        head=node; 
        size++;
    } else {
        doubleList *p=head;
        while (p->next!=NULL) {
            p = p->next;
        }
        p->next = node;
        node->prev = p;
        node->next = NULL;
        size++;

        }

}
void init() {
    head = NULL;
    size = 0;
}
Data* get(int pos) {
    doubleList* d = head;
    for (int i = 0; i<pos; ++i) {
        d = d->next;
    }
    return d->data;
}
int getLength() {
    return size;
}
void DeleteFromList(int position) {
    doubleList* d = head;
    for (int i = 0; i<position; ++i) {
        d = d->next;
    }
    if (d==head) {
        head = d->next;
        if (d->next!=NULL) {
            d->next->prev = NULL;
        }
    size--;
    free(d);
    } else {
    if (d->next!=NULL) {
        d->next->prev = d->prev;
    }
    if (d->prev!=NULL) {
        d->prev->next = d->next;
    } else {
        d->next->prev = NULL;
    }
    size--;
    free(d);
    }
}


void DeleteList() 
{ 
    doubleList* d = head;
    while (d!=NULL) {
        doubleList* p = d->next;
        free(d->data);
        free(d);
        d = p;
    }
    
    
} 