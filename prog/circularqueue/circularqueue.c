/*
 * circularqueue.c
 *
 *  Created on: 03-May-2024
 *      Author: yo
 */
#include <stdio.h>
#include <stdlib.h>
#define SIZE 4
void insert(int x);
int delete();
void display();
int peek();
int queue[SIZE];
int front=-1,rear=-1;
int main(){
	int y;
	insert(10); //0  r=0  300
	insert(20); //1       400
	insert(100); //2 f=2
	insert(200); //3      200
	delete();
	printf("front %d rear %d\n",front,rear);
	insert(300);
	delete();
	delete();
	insert(400);
	printf("front %d rear %d\n",front,rear);
	display();
	y=peek();
	printf("return value peek %d\n",y);
	return 0;
}

void insert(int x){
	if((front==0 && rear==SIZE-1)||(front==rear+1)) //queue is full
	{
		printf("stack is full\n");
		exit(0);
	}
	if(front==-1) //quee is full
	{
		front=0;
	}
	if(rear==SIZE-1) //rear than how to insert
	{
		rear=0;
	}
	else{
		rear=rear+1;
	}
	queue[rear]=x;
}

int delete()
{
	int y;
	if(front==rear) //one element
	{
		front=rear=-1;
	}else if(front==SIZE-1) //front at end how to delete
 	{
		front=0;
	}else{
		front=front+1; //simple
	}
	y=queue[front];
	return y;
}

void display()
{
	int i;
	if(front==-1)
	{
		printf("queue is empty\n");
		exit(0);
	}
	i=front;
	if(front<=rear)
	{
		while(i<=rear)
		{
			printf("elements:%d\n",queue[i]);
			i++;
		}
	}else{
		i=front;
		while(i<=SIZE-1)
		{
			printf("elements first remaining:%d\n",queue[i]);
			i++;
		}
		i=0;
		while(i<=rear)
		{
			printf("elements last remaining:%d\n",queue[i]);
			i++;
		}
	}
}

int peek()
{
	int y;
	if(front==-1)
	{
		printf("empty\n");
		exit(0);
	}
	if(front==SIZE-1)
	{
		front=0;
	}
	y=queue[front];
	return y;
}

