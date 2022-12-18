/*
 * functionpointers.c
 *
 *  Created on: Dec 13, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

void (*fp)();

void disp();

int main()
{

	fp=disp;

	(*fp)();

	return 0;
}

void disp()
{
	printf("hi");
}
