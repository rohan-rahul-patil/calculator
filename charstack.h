typedef struct charstack {
	char c[128];
	int j;
}charstack;

void initc(charstack *q);
void pushc(charstack *q, char y);
char popc(charstack *q);
int emptyc(charstack *q);
int fullc(charstack *q);
