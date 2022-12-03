/*
 * longline.c
 *
 *  Created on: Mar 3, 2022
 *      Author: ubuntu
 */
#include<stdio.h>
#define maxlength 100

int getlinee(char cha[],int lenght);
int copyy(char to[],char from[]);

int main()
{

	int nocharacter=0,prevchar=0;
	char line[maxlength]={'\0'},prtln[maxlength]={'\0'}; //initialize the size
	while((nocharacter=getlinee(line, maxlength))>0)
	{
		if(nocharacter>prevchar)
		{
			prevchar=nocharacter;
			copyy(prtln,line);
		}
	}
	if(prevchar>0)
		printf("%s",prtln);

	return 0;
}
int getlinee(char cha[],int lenght)
{
	int i=0,c=0;
	while((c=getchar())!='\n')
	{
		cha[i]=c;
		i++;
	}
	cha[i]='\0';
	return i;
}
int copyy(char to[],char from[])
{
	int i=0;
	while((to[i]=from[i])!='\0')
		i++;
	return i;
}
