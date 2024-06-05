/*
 * setbits.c
 *
 *  Created on: 29-Jan-2024
 *      Author: yo
 */
#include <stdio.h>
#define SIZE 2
void dectobin(unsigned char _value);
int main(){
	unsigned char read=0x01U,value[SIZE]={0x0U,0x0U},msk=0x01U,i=0,noof1_sum=0,noof0_sum=0;

	for(i=0;i<8;i++)
	{
		value[0]|=(read<<i);
		read^=(0x01U);
		if(value[0] & msk<<(i%8))
		{
			dectobin(value[0]);
			noof1_sum++;
			printf("noofone %d msk:%x ",noof1_sum,msk);

		}else{
			noof0_sum++;
		}
		//or msk<<=1
	}

	if(noof1_sum & 0x01U)
	{
		printf("odd parity %d\n",noof1_sum);
	}else{
		printf("even parity %d\n",noof1_sum);
	}
	i=0;
	/*while(i<SIZE)
	{

		if(noof1_sum!=0 && i!=0)
		{
			store[i]=noof1_sum;
		}else{
			store[i]=value;
		}
		i++;
	}*/

	value[1]=noof1_sum;

	for(i=0;i<SIZE;i++)
	{
		printf("%d\n",value[i]);
	}

	return 0;
}

void dectobin(unsigned char _value)
{
	int rem,i=1,bin=0;
	printf("value :%d",_value);
	while(_value !=0)
	{
		rem=_value%2;
		bin+=rem*i;
		i*=10;
		_value=_value/2;
	}
	printf(" %d\n",bin);
}
