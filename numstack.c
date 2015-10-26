#include <stdio.h>
#include "numstack.h"
#define SIZE 128

void init(numstack *p) {
	p->i = 0;
}

void push(numstack *p, double x) {
	p->a[p->i] = x;
	(p->i)++;
}

double pop(numstack *p) {
	(p->i)--;	
	return p->a[p->i];
}


int empty(numstack *p) {
	return p->i == 0;
}

int full(numstack *p) {
	return p->i == SIZE - 1;
}


