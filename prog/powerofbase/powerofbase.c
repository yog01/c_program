/*
 * powerofbase.c
 *
 *  Created on: Mar 2, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int power(int base,int n);

int main()
{

	int base,n;

	base=n=0;
	base=2;
	for(int i=0;i<11;i++)
	printf("pwer%d\n",power(base, i));

	return 0;
}

int power(int base, int n)
{
	int p=1,i=0;
	for(i=0;i<n;i++)
	{
		p=p*base;
	}
	return p;
}
