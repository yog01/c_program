/*
 * noofcharcter.c
 *
 *  Created on: Feb 21, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int main()
{

	long count=0,nc; //long for 8bytes


	for(count=0;getchar()!='\n';++count);
	printf("no of character %ld",count);

	return 0;
}
