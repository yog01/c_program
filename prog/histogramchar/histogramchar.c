/*
 * histogramchar.c
 *
 *  Created on: Mar 1, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#define size 10
int main()
{
	int c,index,k;
	int ndigit[10]={0};

	c=index=k=0;


	while((c=getchar())!='\n')
	{
		if(c>='0' && c<='9')
		{
			index=c-'0';
			++ndigit[index];
		}
	}
	for(int i=0;i<size;i++)
	{
		for(k=0;k<ndigit[i];k++)
		{
			printf("%d",i);
		}
		if(ndigit[i]!=0)
		printf("\n");
	}
	for(int i=0;i<size;i++)
	{
		printf("%d ",ndigit[i]);
	}

	return 0;
}
