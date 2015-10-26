typedef struct node {
	char c;
	struct node *next;
}node;
	
typedef struct charqueue {
	node *head;
}charqueue;



void qinit(charqueue *q);
void enqueue(charqueue *q, char ch);
char dequeue(charqueue *q);
int qempty(charqueue *q);
int qfull(charqueue *q); 

