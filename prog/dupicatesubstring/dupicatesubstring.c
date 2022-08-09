/*
 * dupicatesubstring.c
 *
 *  Created on: Mar 23, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <strings.h>

int match(char tstr1[],char tstr2[],int l, int s);

int main()
{
	char str1[]="laptop",str2[]="top",matchin=0;
	int l=sizeof(str1)/sizeof(str1[0]);
	int s=sizeof(str2)/sizeof(str2[0]);

	matchin=match(str1, str2,l-1,s-1);

	printf("%d\n",matchin);
	printf("%d %d",l,s);


	return 0;
}
int match(char tstr1[],char tstr2[],int l, int s)
{
	int i,j,ret;
	i=j=ret=0;
	for(i=0;i<=l-s;i++)
	{
		for(j=0;j<s;j++)
		{
			if(tstr1[i+j]!=tstr2[j])
				break;
		}

		if(j==s)
		{
			ret++;
		}
	}
	return ret;
}
