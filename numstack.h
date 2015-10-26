typedef struct numstack {
	double a[128];
	int i;
}numstack;

void init(numstack *p);
void push(numstack *p, double x);
double pop(numstack *p);
int empty(numstack *p);
int full(numstack *p);
