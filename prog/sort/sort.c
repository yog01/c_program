/*
 * sort.c
 *
 *  Created on: Dec 3, 2022
 *      Author: ubuntu
 */
#include <stdio.h>

int main()
{

	int arr[5]={10,2,45,23,5};
	int min,i,j,temp=0;

	for(i=0;i<4;i++)
	{
		min=i;
		for(j=i+1;j<5;j++) //less
		{
			if(arr[j]<arr[min])
			{
				min=j;
			}
		}
		if(i!=min)
		{
			temp=arr[i];
			arr[i]=arr[min];
			arr[min]=temp;
		}
	}

	for(i=0;i<5;i++)
	{
		printf("%d\n",arr[i]);
	}



	return 0;
}
