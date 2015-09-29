#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



#define OPERAND 	1
#define OPERATOR 	2
#define INVALID		3
#define BRACKET		4
#define SIZE		128
typedef struct opdstack {
	double a[128];
	int i;
}opdstack;
typedef struct oprstack {
	char c[128];
	int j;
}oprstack;

void init(opdstack *p);
void initc(oprstack *q);
void push(opdstack *p, double x);
void pushc(oprstack *q, char y);
double pop(opdstack *p);
char popc(oprstack *q);
int empty(opdstack *p);
int emptyc(oprstack *q);
int full(opdstack *p);
int fullc(oprstack *q);
long int factorial(unsigned int n);
double power(double x, double y);
void filehandling(char *inputfile);

int precedance(char c);
double operation(double x, double y, char c);	
double infix(char *string);

void init(opdstack *p) {
	p->i = 0;
}

void initc(oprstack *q) {
	q->j = 0;
}

void push(opdstack *p, double x) {
	p->a[p->i] = x;
	(p->i)++;
}

void pushc(oprstack *q, char y) {
	q->c[q->j] = y;
	(q->j)++;
}

double pop(opdstack *p) {
	(p->i)--;	
	return p->a[p->i];
}

char popc(oprstack *q) {
	(q->j)--;
	return q->c[q->j];
}


int empty(opdstack *p) {
	return p->i == 0;
}

int emptyc(oprstack *q) {
	return q->j == 0;
}

int full(opdstack *p) {
	return p->i == SIZE - 1;
}

int fullc(oprstack *q) {
	return q->j == SIZE - 1;
}
	
typedef struct token {
	int type; 
	union data {
		double num;
		char operator;
		char bracket;
	}d;
}token;
token *getnext(char *string, int *reset);

enum states {SPC, NUM, OPR, END, ERROR, BRAC, DOT};

token *getnext(char *string, int *reset) {
	static int currstate, nextstate;
	static int i;
	int done = 0, d = 0;
	double sum , t = 10;
	token *p = (token *)malloc(sizeof(token));
	if(p == NULL)
		return NULL;
	if(*reset == 1) {
		*reset = 0;
		currstate = SPC;
		i = 0;
	}
	while(1) {
		switch(string[i]) {
			case '1': case'2': case '3': case '4':
			case '5': case'6': case '7': case '8':
			case '9': case'0': 
				nextstate = NUM;
				break;
			case '+': case '-': case '*': case '/':
			case '^':
				nextstate = OPR;
				break;
			case '\0':
				nextstate = END;
				break;
			case ' ':
				nextstate = SPC;
				break;
			case '(': case ')':
				nextstate = BRAC;
				break;
			case '.':
				nextstate = DOT;
				break;
			default:
				nextstate = ERROR;
				break;
		}
		switch(currstate) {
			case SPC:
				switch(nextstate){
					case SPC:
						break;
					case NUM:
						sum = string[i] - '0';	
						break;
					case OPR:
						break;
					case END:
						break;
					case ERROR:
						break;
					case BRAC:
						break;
					case DOT:
						break;
					default:
						break;
				}
				break;
			case NUM:
				switch(nextstate){
					case SPC:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case NUM:
						if(d == 0) 
							sum = sum * 10 + string[i] - '0';
						else
							sum = sum + (string[i] - '0') / t;
							t = t * 10;
						break;
					case OPR:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case END:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case ERROR:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case BRAC:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case DOT:
						break;	
					default:
						break;
				}
				break;
			case OPR:
				switch(nextstate){
					case SPC:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case NUM:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						sum = string[i] - '0';
						done = 1;
						break;
					case OPR:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case END:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case ERROR:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case BRAC:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case DOT:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					default:
						break;
				}
				break;	
			case BRAC:
				switch(nextstate){
					case SPC:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case NUM:
						sum = string[i] - '0';
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case OPR:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case END:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case ERROR:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case BRAC:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case DOT:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					default:
						break;
				}
				break;
			case DOT:
				switch(nextstate){
					case SPC:
						d = 1;
						break;
					case NUM:
						d = 1;
						t = 10;
						sum = sum + (string[i] - '0') / t;
						t = t * 10;	
						break;
					case OPR:
						break;
					case END:
						break;
					case ERROR:
						break;
					case BRAC:
						break;
					case DOT:
						break;
					default:
						break;
				}
				break;
			case END:
				nextstate = END;
				done = 1;
				p = NULL;
				break;
			case ERROR:
				nextstate = ERROR;
				p->type = INVALID;
				done = 1;
				break;
			default:
				break;
		}
		currstate = nextstate;
		i++;
		if(done == 1) {
			return p;
		}
	}
	return NULL;	
}

int precedance(char c) {
	if(c == '(' || c == ')') 
		return 30;
	else if(c == '^')
		return 300;
	else if(c == '*' || c == '/') 
		return 200;
	else
		return 100;
}
double operation(double x, double y, char c) {
	if(c == '*') 
		x = x * y;
	else if(c == '/') 
		x = x / y;
	else if(c == '+') 
		x = x + y;
	else if(c == '-') 
		x = x - y;
	else if(c == '^')
		x = power(x, y);
	return x;
}	
double infix(char *string) {
	int reset = 1, m = 0, n = 0, x, y;
	double w, z;
	char a, b;
	token *p;
	opdstack opdstack;
	oprstack oprstack;
	init(&opdstack);
	initc(&oprstack);
	while((p = getnext(string, &reset)) && !full(&opdstack) && !fullc(&oprstack)) {
		if(m == n || m == n + 1) {
		}
		else
			return INT_MIN;
		if(p->type == OPERAND) {
			push(&opdstack, p->d.num);
			m = m + 1;
		}
		if(p->type == OPERATOR)	{
			n = n + 1;
			if(n == 1) {
				pushc(&oprstack, p->d.operator);
			}
			else {
				a = popc(&oprstack);
				b = p->d.operator;
				if(precedance(b) > precedance(a)) {
					pushc(&oprstack, a);
					pushc(&oprstack, b);
				}
				else {
					w = pop(&opdstack);
					z = pop(&opdstack);
					if(w == 0 && a == '/') {
						return INT_MIN;
					}
					if(m < n) 
						return INT_MIN;
					w = operation(z, w, a);
					push(&opdstack, w);
					pushc(&oprstack, b);
				}
			}		
		}
		
		
		if(p->type == BRACKET) {
			if(p->d.bracket == '(') {
				pushc(&oprstack, p->d.bracket); 
				x = x + 1;
			}
			else if(p->d.bracket == ')') {
				if(emptyc(&oprstack)) 
					return INT_MIN;
				a = popc(&oprstack);
				b = p->d.bracket;
				y = y + 1;
				while(precedance(a) != precedance(b)) {
					w = pop(&opdstack);
					z = pop(&opdstack);
					w = operation(z, w, a);
					push(&opdstack, w);
					a = popc(&oprstack);
				}
			}
				
		}
		
	}
	if(y != x) {
		return INT_MIN;
	}
	if(m <= n) {
		return INT_MIN;
	}
	if(empty(&opdstack)) 
		return INT_MIN;
	while(!(emptyc(&oprstack))) {
		w = pop(&opdstack);
		z = pop(&opdstack);
		a = popc(&oprstack);
		if(w == 0 && a == '/') {
			return INT_MIN;
		}
		w = operation(z, w, a);
		push(&opdstack, w);
	}
	return pop(&opdstack);
}

int main(int argc, char *argv[]) {
	char str[128];
	double y;
	scanf("%[^\n]", str);
	y = infix(str);
	if(y == INT_MIN)
		printf("error...\n");		
	else 
		printf("%lf\n", y);
	return 0;
}
void filehandling(char *inputfile) {
	int ip, x;
	char c[128];
	double y;
	ip = open(inputfile, O_RDONLY | O_CREAT); 
	int	i;
		while(1) {
			i = 0;
			x = read(ip, &c[i], sizeof(char));
			if(x == -1) {
				printf("error\n");
			}
			if(x == 0) {
				c[i] = '\0';
				break;
			}
			while(1) {
				i++;
				x = read(ip, &c[i], sizeof(char));
				if(c[i] == '\n'|| x == 0) {
					c[i] = '\0';
					break;
				}
			}
			y = infix(c);
			printf("%lf\n", y);
		}
}
	
			
				
				
			
			
long int factorial(unsigned int n) {
	long int y;
	if(n == 0) 
		return 1;
	else
	y = n * factorial(n - 1);
	return y;
}

double power(double x, double y) {
	int i;
	double t = 1, s = 1;
	x = x - 1;
	if(y == 0) {
		return 0;
	}
	for(i = 0; i < 100; i++) {
		t = t * (y - i) * x / (i + 1);
		s = s + t;
	}
	return s;
}
	
	
	
















