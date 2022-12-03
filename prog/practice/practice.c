/*
 * practice.c
 *
 *  Created on: Mar 22, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int main()
{
    //char port[10]={"2134"};
    int port[10]={50,48,48,48},nu,base=1;
    unsigned int a=0x01U;
    int i=0;

    while(port[i]!=0)
    {
        nu=port[i]-48;
        nu+=base*10;
        base=nu;
        i++;
    }

    printf("----%d",nu);

    if((!a) & 0x01U)
    {
    	printf("littele indian\n");
    }
    else {
		printf("big indian\n");
	}

    return 0;
}

