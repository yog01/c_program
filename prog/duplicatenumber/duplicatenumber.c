/*
 * duplicatenumber.c
 *
 *  Created on: Sep 13, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

#define n 4

#define debug(x) printf("debug %d",x)

int main()
{

	int a[n]={1,2,2,2},b[n]={0};
	int i=0,j=0;

	for(i=0;i<n-1;++i)
	{
		if(a[i]!=a[i+1])
		{
			b[j]=a[i];
			j++;
		}
	}
	debug(i);
	debug(j);
	b[j]=a[n-1];

	for(i=0;i<n;i++)
	printf("%d",b[i]);



	return 0;
}
