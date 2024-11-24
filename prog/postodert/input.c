/*
 * input.h
 *
 *  Created on: 23-Nov-2024
 *      Author: yo
 */

#include "input.h"


char buf[BUFSIZE];
int bufp=0;

int ex=10;


int getopt(char s[]){

	int c,i;

	while((s[0]=c=_getch())==' '|| c=='\t');

	s[1]='\0';

	if(!isdigit(c)&&c!='.')
		return c;
	i=0;
	if(isdigit(c))
		while(isdigit(s[++i]=c=_getch()));

	if(c=='.')
		while(isdigit(s[++i]=c=_getch()));

	s[i]='\0';

	if(c!=EOF)
		ungetch(c);
	return NUMBER;

}

void ungetch(int a){
	if(bufp>=BUFSIZE)
		printf("ungetch:too many character\n");
	else
		buf[bufp++]=0;
}

int _getch(void){
	return (bufp>0)?buf[--bufp]:getchar();
}
