/*
 * centofren.c
 *
 *  Created on: Feb 13, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int main()
{
	int cen=0,fre=0;

	for(cen=-10;cen<50;cen+=10)
	{
		fre=(cen*9)/5+32;
		printf("centigrate %d frenhite %d\n",cen,fre);
	}

	return 0;
}
