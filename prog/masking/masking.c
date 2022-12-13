/*
 * masking.c
 *
 *  Created on: Dec 5, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int main()
{
	unsigned char a=0x01U;

	//set
	a|=(0x01U<<1);

	printf("2nd bit %x\n",a);

	//reset
	a&=~(0x01U<<1);

	printf("1 bit reset %x\n",a);

	//check
	a&=(0x01U<<0);

	if(0x01U==a)
	{
		printf("1 bit is set\n");
	}
	else
	{
		printf("1 bit is not set\n");
	}

	//toggel

	a^=(0x01U<<0);

	printf("1 bit toggel %x\n",a);

	return 0;
}
