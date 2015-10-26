#include<stdio.h>
#include<stdlib.h>
#include "charqueue.h"

void qinit(charqueue *q) {
	q->head = NULL;
}

void enqueue(charqueue *q, char ch) {
	node *p;
	node *temp;
	p = q->head;
	temp  = (node *)malloc(sizeof(node));
	if(p) {
		while(p->next) {
			p = p->next;
		}
		p->next = temp;
	}
	else
		q->head = temp;
	temp->c = ch;
	temp->next = NULL;
}
		
char dequeue(charqueue *q) {
	char ch;
	node *temp;
	temp = q->head;
	if(q->head->next) 
		q->head = q->head->next;
	else 
		q->head = NULL;
	ch = temp->c;
	free(temp);
	return ch;
}
	
int qempty(charqueue *q) {
	if(q->head)
		return 0;
	else 
		return 1;
}
int qfull(charqueue *q) {
	return 0;
}


