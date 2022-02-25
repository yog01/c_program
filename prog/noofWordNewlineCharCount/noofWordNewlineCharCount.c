/*
 * noofWordNewlineCharCount.c
 *
 *  Created on: Feb 25, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#define INWORD 1
#define OUTWORD 0

int main()
{
	int c,nw,nl,nc,state;
	//nw=1;
	c=nl=nc=nw=0;
	state=OUTWORD;
	while((c=getchar())!='Y')
	{

		if(c=='\n')
			nl++;
		if(c==' '||c=='\n'||c=='\t')
			state=INWORD;
		else
			nc++;
		if(state==INWORD)
		{
			state=OUTWORD;
			nw++;
		}
	}

	printf("no of char %d no of line %d no of word %d",nc,nl,nw);


	return 0;
}
