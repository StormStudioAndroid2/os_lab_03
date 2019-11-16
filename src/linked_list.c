#include "linked_list.h"
#include <stdlib.h>

void AddList(linkedList l,int value) {
    doubleList* node = (doubleList*) malloc(sizeof(doubleList)); 
    node->data=value; 
    if (l.head==NULL) {
        node->next=node;
        node->prev=node; 
        l.head=node; 
        l.size++;
    }
    else {
        doubleList *p=l.head;
        while (p->next!=NULL) {
            p = p->next;
        }
        p->next = node;
        node->prev = p;
        node->next = NULL;
                l.size++;

        }

}
void init(linkedList l) {
    l.head = NULL;
    l.size = 0;
}
int get(linkedList l, int pos) {
    if (pos<0) {return 0;}
    doubleList* d = l.head;
    for (int i = 0; i<pos; ++i) {
        d = d->next;
    }
    return d->data;
}
int getLength(linkedList l) {
    return l.size;
}
int DeleteFromList(linkedList l,int position) {
    if (l.head==NULL) { return 0; }
    if (l.head==l.head->next) {
        free(l.head); 
        l.head=NULL;
        l.size--;
    } else {
        doubleList *a=l.head;
        for (int i=position; i>1; i--) a=a->next;
        if (a==l.head) l.head=a->next;
        a->prev->next=a->next; 
        a->next->prev=a->prev;
        l.size--;
        free(a);
    }

}


void deleteList(linkedList l, doubleList head) 
{ 
    if (l.head == NULL) 
        return; 
    deleteList(l,*l.head->next);  
    free(l.head); 
} 