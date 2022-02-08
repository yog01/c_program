/*
 * fibonic.c
 *
 *  Created on: Feb 8, 2022
 *      Author: ubuntu
 */
#include <stdio.h>


int fib(int n);

int main()
{
	int a=0,value=0;

	printf("Enter a number\n");
	scanf("%d",&a);

	value=fib(a);

	printf("Fib is \n %d",value);


	return 0;
}

int fib(int n)
{
	if(n<=1)
		return n;

	return (fib(n-1)+fib(n-2));
}
