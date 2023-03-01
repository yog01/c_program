#include <stdio.h>
#include <stdlib.h>

int main()
{

	int *arr[2];

	int i=0;



		for(i=0;i<2;++i)
		{
			arr[i]=calloc(0,3*sizeof(*arr[i]));//2 row 3 col
			if(NULL==arr[i])
			{
				perror("dynamic fail\n");
			}
		}

	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			arr[i][j]=100;
			printf("%d",arr[i][j]);
		}
	}


		for(i=0;i<2;++i)
		free(arr[i]);


	return 0;
}
