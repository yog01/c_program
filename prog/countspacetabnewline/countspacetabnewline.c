/*
 * countspacetabnewline.c
 *
 *  Created on: Feb 22, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int main()
{
	int c,sp=0,tb=0,nl=0,cha=0;

	while((c=getchar())!='y')
	{
		switch(c)
		{
		case ' ':
			sp++;
			break;
		case '\t':
			tb++;
			break;
		case '\n':
			nl++;
			break;
		default:
			cha++;
			break;
		}
	}

	printf("space %d, tab %d, newline %d, chaaracter %d",sp,tb,nl,cha);

	return 0;
}
