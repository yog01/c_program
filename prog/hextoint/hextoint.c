/*
 * hextoint.c
 *
 *  Created on: Oct 15, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <math.h>
#define SIZE 5

void strto_int(unsigned int *arr,int size,unsigned int *store);
void htoi(unsigned int *store,int size);
int main()
{
	unsigned int arr[SIZE]={'0','x','A','1','\0'};
	unsigned int y[SIZE]={0};
	int i=0;
	strto_int(arr,SIZE,y);
	htoi(y,SIZE);

	for(i=0;i<SIZE;++i)
	{
		printf("%d\n",y[i]);
	}
	return 0;
}

void strto_int(unsigned int *arr,int size,unsigned int *store)
{

	int i=1;
	while(arr[i]!='\0')
	{
		if(arr[0]=='0'|| (arr[i]=='x' || arr[i]=='X'))
		{
			if(arr[i]>='0' && arr[i]<='9')
			{
				store[i]=arr[i]-'0';
			}
			else if(arr[i]>='A' && arr[i]<='F')
			{
				store[i]=10+arr[i]-'A';
			}
			else
			{
				store[i]=0;
			}
			++i;
		}
		else {
			store[i]=0;
		}
	}

}

void htoi(unsigned int *store,int size)
{
	unsigned int z=0U,i=size-2;
	double base=16,i_nte=0;
	//check for interger  conveted
	while(i>=2)
	{
		i_nte+=(store[i]*pow(base,z));
		z++;
		i--;
	}
	printf("%lf\n",i_nte);
}
