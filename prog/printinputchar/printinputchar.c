/*
 * printinputchar.c
 *
 *  Created on: Feb 18, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int main()
{
	int c; //to check for EOF value

	c=getchar();

	while(c!=EOF)
	{
		putchar(c);
		c=getchar();
	}


	return 0;
}
