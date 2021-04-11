#include<stdio.h>
#define SIZE 50
void push(int i);
int pop();
	int value;
	int *p,*tos,stack[SIZE];
int main()
{
	

	p=stack;
	tos=stack;

	do{
		printf("Enter a value\n");
		scanf("%d",&value);
		if(value !=0)
		{
			push(value);
		}
		else
		{
			printf("Pooped vale is %d",pop());
		}

	}while(value !=-1);



	return 0;
}
void push(int i)
{
	p++;
	if(p==tos+SIZE)
	{
		printf("Statck is overflow\n");
	}

	*p=i;
}

int pop()
{
	int j=0;
	if(p==tos)
	{
		printf("statck is underflow\n");
	}
	
	p--;//update address to below
	return *(p+1);// give vlaue above stack member

}







