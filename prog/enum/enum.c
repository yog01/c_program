/*
 * enum.c
 *
 *  Created on: 08-Sep-2023
 *      Author: yo
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
typedef enum abc{ONE,TWO,THREE}abcd;

int main(){
	abcd i;
	abcd arr[5];
	for(i=ONE;i<=THREE;++i)
	{
		arr[i]=i;
		printf("%d",arr[i]);
	}




	return 0;
}

