/*
 * fact.c
 *
 *  Created on: Feb 8, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int fact(int n)
{
	if(n==0)
		return 1;

	return n*fact(n-1);
}

int main()
{
	int a;

	a=fact(4);

	printf("%d",a);

	return 0;
}

