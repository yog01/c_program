/*
 * practice.c
 *
 *  Created on: Mar 22, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define NOOFTHREAD 3

pthread_t th_id[NOOFTHREAD];

void *th_fn(void *arg);
int threadcreatdettach(pthread_t *th_id,void *arg);
int main()
{
	int i=0,*data[NOOFTHREAD];

	for(i=0;i<NOOFTHREAD;++i)
	{
		data[i]=(int *)calloc(0,sizeof(*data[i]));
		*data[i]=i;
		threadcreatdettach(th_id+i, data[i]); //address allocated on array access by arr[0]
	}
	return 0;
}
int threadcreatdettach(pthread_t *th_id,void *arg)
{
	pthread_attr_t att;
	void *ptr_dta;
	int *ptr_data;
	pthread_attr_setdetachstate(&att,PTHREAD_CREATE_JOINABLE);
	pthread_create(th_id, 0, th_fn, arg);
	pthread_join(*th_id, &ptr_dta);
	ptr_data=(int *)ptr_dta;
	free(arg);
	printf("return allocaated address%p\n",ptr_data);
	return 0;
}
void *th_fn(void *arg)
{
	int *data=(int*)arg;
	while(*data!=0)
	{
		printf("%d\n",--(*data));
	}

	return arg;
}
