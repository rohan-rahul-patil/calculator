#include <stdio.h>
#include "charstack.h"
#define SIZE 128

void initc(charstack *q) {
	q->j = 0;
}

void pushc(charstack *q, char y) {
	q->c[q->j] = y;
	(q->j)++;
}

char popc(charstack *q) {
	(q->j)--;
	return q->c[q->j];
}

int emptyc(charstack *q) {
	return q->j == 0;
}

int fullc(charstack *q) {
	return q->j == SIZE - 1;
}

