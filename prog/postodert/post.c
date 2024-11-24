/*
 * post.c
 *
 *  Created on: 23-Nov-2024
 *      Author: yo
 */

#include "input.h"

#define MAXOP 100

#define MAXVAL 100

extern int ex;

double pop(void);
void push(double f);

int sp=0 ;
double val[MAXVAL];

int main(){
	int type;
	double op2;
	char s[MAXOP];

	while((type=getopt(s))!=EOF){
		switch(type){
			case NUMBER:
				push(atof(s));
				break;
			case '+':
				push(pop()+pop());
				break;
			case '*':
				push(pop()*pop());
				break;
			case '-':
				op2=pop();
				push(pop()-op2);
				break;
			case '/':
				op2=pop();
				if(op2!=0.0)
					push(pop()/op2);
				else
					printf("error:divide by zero\n");
				break;
			case '\n':
				printf("\t%.8g\n",pop());
				break;
			default:
				ex=20;
				printf("unknow command %s %d\n",s,ex);
				break;
		}
	}

	return 0;
}

/*void push(double f)
{
	if(sp<MAXVAL)
		val[sp++]=f;
	else
		printf("error:stack full%g\n",f);
}*/


void push(double f){
	if(sp<MAXVAL)
	{
		val[sp++]=f;
	}else{
		printf("stack overflow\n");
	}
}

/*double pop(void)
{
	if(sp>0)
		return val[--sp];
	else{
		printf("error: stack empty\n");
		return 0.0;
	}
}*/

double pop(void){
	if(sp>0)
		return val[--sp];
	else
	{
		printf("error: stack empty\n");
		return 0.0;
	}
}
