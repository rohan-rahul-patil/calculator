#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "numstack.h"
#include "charstack.h"
#include "charqueue.h"


#define OPERAND 	1
#define OPERATOR 	2
#define INVALID		3
#define BRACKET		4
#define FUNCTION	5
#define SIZE		128

struct variable v;

char ans[64];
double answer;

int ib = 10;
int ob = 10;
void outputbase(double a, char *str, int base);
double inputbase(charstack *s, charqueue *q, int base);
void doubletostring(double d, char *str);
long int factorial(unsigned int n);
double power(double x, double y);
void filehandling(char *inputfile);
char *stringreplace(char *t, char *o, char *n);
char *newstring(char *str);
int precedance(char c);
double operation(double x, double y, char c);	
double infix(char *string);
double sin(double x);
void varassignment(struct variable *v, char *str);

char *stringreplace(char *t, char *o, char *n) {                      //replaces one string with given string
	int i, j, k;
	char *s = o;
	char *p = n;
	char *l;
	char * text;
	text = t;
	while(*t != '\0') {
		if(*t == *o) {
			t++;
			l = t;
			o++;
		}
		else {
			if(o == s) {
				t++;
				l = t;
			}
			else {
				o = s;
			}
		}
		if(*o == '\0') {
			t = t - (o - s);
			o = s;
			while(1) {
				*t = *n;
				t++;
				n++;
				if(*n == '\0') {
					break;
				}
				if( t == l) {
					i = 0;
					while(l[i] != '\0') {
						i = i + 1;
					}
					while(i != -1) {
						l[i+1] = l[i];
						i--;
					}
				l = l + 1;
				}
			}
			if(t != l && *n == '\0') {
				j = l - t;
				i = 0;
				while(l[i] != '\0') {
					i = i + 1;
				}
				for(k = 0; k <= i; k++) {
					l[k - j] = l[k]; 
				}
			}
		}
		n = p;
	}
	return text;
}

char *newstring(char *str) {					// used for execution of functions
	char *s;
	s = stringreplace(str, "sin(", "(s");
	s = stringreplace(s, "cos(", "(c");
	s = stringreplace(s, "tan(", "(t");
	s = stringreplace(s, "cot(", "(o");
	s = stringreplace(s, "sec(", "(y");
	s = stringreplace(s, "cosec(", "(x");
	s = stringreplace(s, "e", "2.71828");
	s = stringreplace(s, "ln(", "(l");
	s = stringreplace(s, "log(", "(g");
	return s;
}

int i = -1;
int f = -1;

struct variable {
	char varname[128][16];
	char varvalue[128][32];
}; 				

void varassignment(struct variable *v, char *str) {                      //for assignment of variables on commandline
	int j = 0, k = 0, check = 0, x;
	int y = 0;
	while(str[j] != '\0') {
		if(str[j] == '	' || str[j] == ' ') {
			j++;
		}
		else if(str[j] == '\0')
			break;
		else if(((47 < str[j] && str[j] < 58) || str[j] == 46) || (str[j] == 'a' && str[j + 1] == 'n' && str[j + 2] == 's') || (str[j] 			== 45)) {
			if(check == 0) 
				break;
			i++;
			k = 0;
			if(str[j] == 'a' && str[j + 1] == 'n' && str[j + 2] == 's') {
				while(ans[k] != '\0') {
					v->varvalue[i][k] = ans[k];
					k++;
				}
				v->varvalue[i][k] = '\0';
				j = j + 3;
				check = 0;
				for(k = 0; k < f; k++) {
					x = strcmp(v->varname[k], v->varname[f]);
					if(x == 0) {
						strcpy(v->varvalue[k], v->varvalue[f]);
						f--;
						i--;
						break;
					}
				}	
			}
			else {
				while(str[j] != '\0' && str[j] != ' ' && str[j] != '	') {
					v->varvalue[i][k] = str[j];
					k++;
					j++;
				}
				v->varvalue[i][k] = '\0';
				check = 0;
				for(k = 0; k < f; k++) {
					x = strcmp(v->varname[k], v->varname[f]);
					if(x == 0) {
						strcpy(v->varvalue[k], v->varvalue[f]);
						f--;
						i--;
						break;
					}
				}
				if(y == 1) {
					y = 0;
					k = 0;
					while(v->varvalue[i][k] != '\0') {
						y = y * 10 + v->varvalue[i][k] - '0';
						k++;
					}
					ib = y;
					y = 0;
					i--;
				}
				else if(y == -1) {
					y = 0;
					k = 0;
					while(v->varvalue[i][k] != '\0') {
						y = y * 10 + v->varvalue[i][k] - '0';
						k++;
					}
					ob = y;
					y = 0;
					i--;
				}
						
				if(str[j] == '\0')
					break;
				j++;
			}
		}
		else if(str[j] == '=') {
			check = 1;
			j++;
		}
		else if((64 < str[j] && str[j] < 91) || (96 < str[j] && str[j] < 123)) {
			if(str[j] == 'i' && str[j + 1] == 'b') {
				y = 1;
				j = j + 2;
				continue;
			}
			else if(str[j] == 'o' && str[j + 1] == 'b') {
				y = -1;
				j = j + 2;
				continue;
			}
			else {			
				f++;
				k = 0;	
				while(str[j] != '\0' && str[j] != ' ' && str[j] != '	') {
					if(str[j] == '=') {
						j--;
						break;
					}
					v->varname[f][k] = str[j];
					k++;
					j++;
				}
				v->varname[f][k] = '\0';
				j++;
			}
		}
	
	}
}	
typedef struct token {
	int type; 
	union data {
		double num;
		char operator;
		char bracket;
		char funct;
	}d;
}token;
token *getnext(char *string, int *reset);

enum states {SPC, NUM, OPR, END, ERROR, BRAC, DOT, FUNCT};

token *getnext(char *string, int *reset) {				//converts a given string into tokens and returns the tokens
	static int currstate, nextstate;
	static int i;
	int done = 0, d = 0;
	double t = 10.0;
	static double sum = 0.0;
	static charstack s;
	static charqueue q;
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
			case '9': case'0': case'A': case'B': case'C':
			case 'D': case'E': case'F': 
				nextstate = NUM;
				break;
			case '+': case '-': case '*': case '/':
			case '^':
				nextstate = OPR;
				break;
			case '\0':
				nextstate = END;
				break;
			case ' ': case '	':
				nextstate = SPC;
				break;
			case '(': case ')':
				nextstate = BRAC;
				break;
			case '.':
				nextstate = DOT;
				break;
			case 's': case 'c': case 't': case 'o': case 'x': case 'y': case 'l': case 'g':
				nextstate = FUNCT;
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
						initc(&s);
						qinit(&q);
						if(ib == 10)
							sum = (double)(string[i] - '0');
						else
							pushc(&s, string[i]);	
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
					case FUNCT:
						break;
					default:
						break;
				}
				break;
			case NUM:
				switch(nextstate){
					case SPC:
						p->type = OPERAND;
						if(ib == 10)
							p->d.num = sum;
						else 
							p->d.num = inputbase(&s, &q, ib);
						sum = 0.0;
						done = 1;
						break;
					case NUM:
						if(ib == 10) {
							if(d == 0) 
								sum = sum * 10.0 + (double)(string[i] - '0');
							else
								sum = sum + (double)(string[i] - '0') / t;
								t = t * 10.0;
						}
						else {
							if(d == 0) 
								pushc(&s, string[i]);
							else
								enqueue(&q, string[i]);
						}
						break;
					case OPR:
						p->type = OPERAND;
						if(ib == 10) 
							p->d.num = sum;
						else
							p->d.num = inputbase(&s, &q, ib);
						sum = 0.0;
						done = 1;
						break;
					case END:
						p->type = OPERAND;
						if(ib == 10) 
							p->d.num = sum;
						else
							p->d.num = inputbase(&s, &q, ib);
						sum = 0.0;
						done = 1;
						break;
					case ERROR:
						p->type = OPERAND;
						if(ib == 10) 
							p->d.num = sum;
						else
							p->d.num = inputbase(&s, &q, ib);
						sum = 0.0;
						done = 1;
						break;
					case BRAC:
						p->type = OPERAND;
						if(ib == 10)
							p->d.num = sum;	
						else
							p->d.num = inputbase(&s, &q, ib);
						sum = 0.0;
						done = 1;
						break;
					case DOT:
						break;	
					case FUNCT:
						p->type = OPERAND;
						if(ib == 10)
							p->d.num = sum;
						else
							p->d.num = inputbase(&s, &q, ib);
						sum = 0.0;
						done = 1;
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
						initc(&s);
						qinit(&q);
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						if(ib == 10) 
							sum = (double)(string[i] - '0');
						else
							pushc(&s, string[i]);
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
					case FUNCT:
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
						initc(&s);
						qinit(&q);
						if(ib == 10)
							sum = (double)(string[i] - '0');
						else
							pushc(&s, string[i]);
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
					case FUNCT:
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
						t = 10.0;
						if(ib == 10) 
							sum = sum + (double)(string[i] - '0') / t;
						else
							enqueue(&q, string[i]);
						t = t * 10.0;
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
					case FUNCT:
						break;
					default:
						break;
				}
				break;
			case FUNCT:
				switch(nextstate){
					case SPC:
						p->type = FUNCTION;
						p->d.funct = string[i-1];
						done = 1;
						break;
					case NUM:
						initc(&s);
						qinit(&q);
						if(ib == 10)
							sum = (double)(string[i] - '0');
						else
							return NULL;
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case OPR:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case END:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case ERROR:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case BRAC:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case DOT:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case FUNCT:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
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

int precedance(char c) {				//checks the precedance of various operations
	if(c == '(' || c == ')') 
		return 30;
	if(c == 's' || c == 'c' || c == 't' || c == 'o' || c == 'x' || c == 'y' || c == 'l' || c == 'g')
		return 50;
	else if(c == '^')
		return 300;
	else if(c == '*' || c == '/') 
		return 200;
	else
		return 100;
}
double operation(double x, double y, char c) {		//performs arithmatic operations
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
double infix(char *string) {				//evaluates the infix expression 
	int reset = 1, m = 0, n = 0, x, y, check = 0;
	double w, z;
	char a, b;
	int state = OPR;
	token *p;
	numstack numstack;
	charstack charstack;
	init(&numstack);
	initc(&charstack);
	while((p = getnext(string, &reset)) && !full(&numstack) && !fullc(&charstack)) {
		/*if(m == n || m == n + 1) {
		}
		else
			return INT_MIN;*/
		if(p->type == OPERAND) {
			if(check == 1) {
				w = p->d.num;
				w = w * -1;
				push(&numstack, w);
				check = 0;
			}
			else {
				push(&numstack, p->d.num);
				m = m + 1;
			}
			state = NUM;	
		}
		if(p->type == OPERATOR)	{
			n = n + 1;
			if(emptyc(&charstack) && state != OPR) {
				pushc(&charstack, p->d.operator);
			}
			else if(state == OPR || state == BRAC || state == FUNCT) {
				if( p->d.operator == '-')
					check = 1;
				else
					return INT_MIN;
			}
			else {
				a = popc(&charstack);
				b = p->d.operator;
				if(precedance(b) > precedance(a)) {
					pushc(&charstack, a);
					pushc(&charstack, b);
				}
				else {
					w = pop(&numstack);
					z = pop(&numstack);
					if(w == 0 && a == '/') {
						return INT_MIN;
					}
					//if(m < n) 
						//return INT_MIN;
					w = operation(z, w, a);
					push(&numstack, w);
					pushc(&charstack, b);
				}
			}
			state = OPR;		
		}
		
		if(p->type == FUNCTION) {
			pushc(&charstack, p->d.funct);
			state = FUNCT;
		}
		if(p->type == BRACKET) {
			if(p->d.bracket == '(') {
				pushc(&charstack, p->d.bracket); 
				x = x + 1;
			}
			else if(p->d.bracket == ')') {
				//if(emptyc(&charstack)) 
					//return INT_MIN;
				a = popc(&charstack);
				b = p->d.bracket;
				y = y + 1;
				while(precedance(a) != precedance(b)) {
					if(a == 's' || a == 'c' || a == 't' || a == 'o' || a == 'x' || a == 'y' || a == 'l' || a == 'g') {
						w = pop(&numstack);
						//printf("w before %lf\n",w);
						w = sin(w);//opfunction(w, a);	
					}
					else {
						w = pop(&numstack);
						z = pop(&numstack);
						w = operation(z, w, a);
					}
					//printf("w after %lf\n", w);
					push(&numstack, w);
					a = popc(&charstack);
				}
			}
		state = BRAC;		
		}
		
	}
	/*if(y != x) {
		return INT_MIN;
	}
	if(m <= n) {
		return INT_MIN;
	}*/
	/*if(empty(&numstack)) 
		return INT_MIN;*/
	while(!(emptyc(&charstack))) {
		w = pop(&numstack);
		z = pop(&numstack);
		a = popc(&charstack);
		if(w == 0 && a == '/') {
			return INT_MIN;
		}
		w = operation(z, w, a);
		push(&numstack, w);
	}
	return pop(&numstack);
}

int main(int argc, char *argv[]) {
	char str[128];
	char *s, c;
	int x, check = 0, j;
	if(argc == 2) {
		filehandling(argv[1]);
		return 0;
	}
	while(1) {
		j = 0;
		while((c = getchar()) != '\n') {
			str[j] = c;
			j++;
		}
		str[j] = '\0';
		s = str;
		if(str[0] == 'q')
			break;
		if(str[0] == 'a') {	
			j = 0;
			while((c = getchar()) != '\n') {
				str[j] = c;
				j++;
			}
			str[j] = '\0';
			//printf("%s\n", str);
			varassignment(&v, str);	
			check = 1;
			continue;
		}
		if(check == 1) {
			if(i != -1) {
				s = stringreplace(str, (&v)->varname[0], (&v)->varvalue[0]);
				for(x = 1; x <= i; x++) 
					s = stringreplace(s, (&v)->varname[x], (&v)->varvalue[x]);
				check = 0;
			}
		}
		s = newstring(s);
		//printf("%s\n",s);
		answer = infix(s);
		if(ob == 10) 
			doubletostring(answer, ans);
		printf("\n");
		if(ob != 10) {
			outputbase(answer, ans, ob);
		}
		printf("ans = %s\n", ans);
		if(answer == INT_MIN)
			printf("error...\n");		
		/*else 
			printf("%.15lf\n", answer);*/
		printf("\n");
	}
	return 0;
}
void filehandling(char *inputfile) {			//takes infix expression from a file
	int ip, x, check = 0;
	char c[128];
	double y;
	char *s;
	ip = open(inputfile, O_RDONLY | O_CREAT); 
	int k;
	s = c;
		while(1) {
			k = 0;
			x = read(ip, &c[k], sizeof(char));
			if(x == -1) {
				printf("error\n");
			}
			if(x == 0) {
				c[k] = '\0';
				break;
			}
			while(1) {
				k++;
				x = read(ip, &c[k], sizeof(char));
				if(c[k] == '\n'|| x == 0) {
					c[k] = '\0';
					break;
				}
			}
			if(check == 2) {
				if(i != -1) {
					s = stringreplace(s, (&v)->varname[0], (&v)->varvalue[0]);
					for(x = 1; x <= i; x++) 
						s = stringreplace(s, (&v)->varname[x], (&v)->varvalue[x]);
				}
				check = 0;
			}
			if(check == 1) {
				varassignment(&v, s);
				check = 2;
				continue;
			}
			if(s[0] == 'a') {
				check = 1;
				continue;
			}
			s = newstring(c);
			y = infix(s);
			if(ob == 10) {
				doubletostring(y, ans);
				printf("%s\n", ans);
			}
			else {
				outputbase(y, ans, ob);
				printf("(%s)%d", ans, ob);
			}
			//printf("%.15lf\n", y);
			
		}
	close(ip);
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
double sin(double x) {
	x = x*x;
	return x;
}

void doubletostring(double d, char *str) {				//converts a number to string
	charstack s;
	initc(&s);
	double k;
	char c;	
	int x, t = 10, y, sign = 0;
	long int z;
	if(d < 0) {
		sign = 1;
		d = (-1) * d;
	}
	x = (int)(d);
	while(x > 0) {
		y = x - (x / t) * t;
		x = x / t;
		c = '0' + (char)(y);
		pushc(&s, c);
	}
	if(sign == 1) {
		pushc(&s, '-');
	}
	y = 0;
	while(!emptyc(&s)) {
		c = popc(&s);
		str[y] = c;
		y++;
	}
	x = (int)(d);
	d = d - x;
	k = d;
	t = 0;
	while(t < 15) {
		k = k * 10;
		z = (long int)(k);
		t++;
	}
	if(z == 0) {
		str[y] = '\0';
		return;
	}
	str[y] = '.';
	y++;
	t = 0;
	while(t < 15) {
		x = d * 10;
		d = d * 10 - x;
		c = '0' + (char)(x);
		str[y] = c;
		y++;
		t++;
	}
	str[y] = '\0';
}
	

double inputbase(charstack *s, charqueue *q, int base) {		//converts a number of a certain base to base 10
		char c;
		double d = 0;
		double b = 1;
		while(!emptyc(s)) {
			c = popc(s);
			if(47 < c  && c < 58)	
				d = d + (c - '0') * b;
			else if(64 < c && c < 71)
				d = d + (c - 55) * b;
			b = b * base;
		}
		b = 1.0 / (double)(base);
		while(!qempty(q)) {
			c = dequeue(q);
			if(47 < c && c < 58)
				d = d + (c - '0') * b;
			else if(64 < c && c < 71) 
				d = d + (c - 55) * b;
			b = b / base;
		}
		return d;
}
				
void outputbase(double a, char *str, int base) {
	int x, y, i, sign = 0;
	long int k;
	char c;
	double d;
	charstack s;
	charqueue q;
	initc(&s);
	qinit(&q);
	if(a < 0) {
		a = -1 * a;
		sign = 1;
	}
	x = (int)(a);
	while(x != 0) {
		y = x % base;
		if(0 <= y && y < 10) {
			c = y + '0';
		}
		else if(10 <= y && y < 16) {
			c = y + 55;
		}
		pushc(&s, c);
		x = x / base;
	}
	if(sign == 1) 
		pushc(&s, '-');
	a = a - (int)(a);
	d = a;
	for(i = 0; i < 15; i++) {
		a = a * base;
		y = a;
		a = a - y;
		if(0 <= y && y < 10) {
			c = y + '0';
		}
		else if(10 <= y && y < 16) {
			c = y + 55;
		}
		enqueue(&q, c);
	}

	for(i = 0; i < 15; i++) {
		k = (long int)(d * 10);
	}
	y = 0;
	while(!emptyc(&s)) {
		str[y] = popc(&s);
		y++;
	}
	if(k == 0) {
		str[y] = '\0';
		return;
	}
	str[y] = '.';
	y++;
	while(!qempty(&q)) {
		str[y] = dequeue(&q);
		y++;
	}
	str[y] = '\0';
}	
		
		
		
		
	
	
	
















