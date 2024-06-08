/*
 * threadfncptr.c
 *
 *  Created on: 20-Dec-2023
 *      Author: yo
 */
/*
 * practice_th.c
 *
 *  Created on: 02-Dec-2023
 *      Author: yo
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//#define PRAC
#ifndef PRAC
extern int errno;

#define SIZE 2

pthread_t th[SIZE],th1,th2;
void *(*fn_ptr)(void *arg);

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutexattr_t mutexattr;
static void *th_fn(void *arg)
{
	pthread_mutex_lock(&lock);
	printf("in th_fn\n");
	unsigned int da=*(unsigned int *)arg;

	free(arg);
	unsigned int *ptr=calloc(1,sizeof(unsigned int));
	*ptr=da+10;
	printf("val %d address %p\n",*ptr,arg);
	pthread_mutex_unlock(&lock);
	//pthread_exit((void *)arg);
	return (void *)ptr;
}
int _pthread_create(pthread_t *th,void *arg){

	int chk=0;
	unsigned int *_ptr=(unsigned int *)arg;

	printf("in _pthread_create\n");

	unsigned int *_ptrm=calloc(1,sizeof(unsigned int));

	memcpy(_ptrm,_ptr,sizeof(unsigned int));
	fn_ptr=th_fn;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	printf("heap address %p sz %ld thid %p\n",_ptrm,sizeof(*_ptrm),th);
	chk=pthread_create(th, &attr, fn_ptr, (void *)_ptrm);
	if(chk!=0)
	{
		perror("pthread_create fail\n");
		exit(EXIT_FAILURE);
		printf("%d",errno);
		chk=-1;
	}

	return chk;
}
int main(){

	unsigned int val=10,i=0,chk=0;
	void *ret_reslt;
	unsigned int *ptr;
	pthread_mutexattr_init(&mutexattr);
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&lock, &mutexattr);
	while(i<SIZE)
	{
		val+=i;
		chk=_pthread_create(th+i,&val);
		if(chk!=0)
		{
			printf("_pthread_create fail\n");
			exit(EXIT_FAILURE);
		}
		ret_reslt=NULL;
		printf("waiting for thread to join %d\n",i);
		chk=pthread_join(*(th+i), (void **)&ret_reslt);
		if(chk<0)
		{
			perror("thread join fail\n");
			printf("error no is %d\n",errno);
			exit(EXIT_FAILURE);
		}
		ptr=(unsigned int*)ret_reslt;
		printf("val %d and address %p\n",*ptr,ptr);
		free(ptr);

		i++;
	}
	pthread_mutexattr_destroy(&mutexattr);
	pthread_mutex_destroy(&lock);
	return 0;
}
#endif
