/*
 * linkedlistqueue.c
 *
 *  Created on: Aug 22, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <stdlib.h>

struct qnode{
	int data;
	struct qnode *next;
};

struct queue{
	struct qnode *front, *rear;
};

struct qnode *create_node(int k);
struct queue *create_queue();
void enque(struct queue *q,int k);
void deque(struct queue *q);
void display(struct queue *q);
int main()
{
	struct queue *q=create_queue();
	enque(q, 10);
	enque(q, 20);
	deque(q);
	display(q);


	return 0;
}

void display(struct queue *q)
{
	struct queue *temp=q;
	while(temp!=NULL)
	{
		printf("values %d\n",temp->front->data);
		temp=temp->front->next;
	}
}

struct queue *create_queue()
{
	struct queue *temp=(struct queue *)malloc(sizeof(struct queue));
	temp->front=NULL;
	temp->rear=NULL;
	return temp;
}
struct qnode *create_node(int k)
{
	struct qnode *temp=(struct qnode *)malloc(sizeof(struct qnode));
	temp->data=k;
	temp->next=NULL;
	return temp;
}

void enque(struct queue *q,int k)
{
	struct qnode *temp=create_node(k);
	if(q->rear==NULL)
	{
		q->rear=q->front=temp;
	}
	q->rear->next=temp; //linking of node
	q->rear=temp;
}

void deque(struct queue *q)
{
	struct qnode *temp=q->front;
	if(q->front==NULL)
	{
		return;
	}
	q->front=q->front->next;
	free(temp);
}


