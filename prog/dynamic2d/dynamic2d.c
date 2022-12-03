/*
 * dynamic2d.c
 *
 *  Created on: Mar 23, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int **arr,row=0,col=0,i=0,j=0;

	printf("Enter the row and col\n");
	scanf("%d%d",&row,&col);

	arr=(int **)malloc(row*sizeof(int));
	for(i=0;i<row;i++)
	{
		arr[i]=(int *)malloc(col*sizeof(int));
	}

	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			printf("Enter the element a[%d][%d]\n",i,j);
			scanf("%d",&arr[i][j]);
		}
	}

	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			printf("Enter the element a %d\n",arr[i][j]);
		}
	}

	for(i=0;i<row;i++)
	{
		free(arr[i]);
	}

	free(arr);


	return 0;
}
