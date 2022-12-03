/*
 * linkedlistpriorityqueue.c
 *
 *  Created on: Aug 22, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <stdlib.h>

struct queue{
	int data;
	int priority;
	struct queue* next;
};

struct queue *front=NULL;

void insertt(int value,int pr);
void displayy();
int main()
{
	int choice,num,pr;

	while(1)
	{
		printf("1.insert in queue\n");
		printf("2.display\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				printf("Enter the value\n");
				scanf("%d",&num);
				printf("Enter the value %d priority\n",num);
				scanf("%d",&pr);
				insertt(num, pr);
				break;
			case 2:
				printf("Enter the value\n");
				displayy();
				break;
			default:

				break;
		}
	}

	return 0;
}

void insertt(int value,int pr)
{
	struct queue *temp;
	struct queue *ptr=(struct queue*)malloc(sizeof(struct queue));

	if(front==NULL||ptr->priority<front->priority)
	{
		ptr->data=value;
		ptr->next=front;
		front=ptr;
	}
	else
	{
		temp=front;
		while(temp!=NULL && temp->next->priority<=ptr->priority)
			temp=temp->next;
		ptr->data=value;
		ptr->next=temp->next;
		temp->next=ptr;
	}

}
void displayy()
{
	struct queue *temp;

	temp=front;

	while(temp!=NULL)
	{
		printf("value %d and priority %d\n",temp->data,temp->priority);
		temp=temp->next;
	}
}
