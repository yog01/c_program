/*
 * stringcount.c
 *
 *  Created on: Sep 30, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int string_count(char *arr);

int main()
{
	char arr[]="qqqwwwee";
	int count=0;

	count=string_count(arr);

	printf("no of characters%d\n",count);

	return 0;
}

int string_count(char *arr)
{
	int i=0;

	while(arr[i]!='\0')
		++i;

	return i;
}
