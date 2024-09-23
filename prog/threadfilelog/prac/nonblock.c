/*
 * thre.c
 *
 *  Created on: 05-May-2024
 *      Author: yo
 */
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
//file open O_NONBLOCK read write
#define nonblock
#ifndef nonblock
#define NOTH 2
#define SIZE 5
extern int errno;
typedef struct {
	int fp,arg;
	char buffer[SIZE];
}fl;

static void *th_fun(void *arg){
	fl *ptr=(fl *)arg;
	int noofbytes=0;
	printf("th_fun:addrss:%p data:%d fp:%d\n",ptr,ptr->arg,ptr->fp);
	snprintf(ptr->buffer,SIZE,"1:%d",ptr->arg);
	noofbytes=write(ptr->fp,(void *)ptr->buffer,SIZE);
	if(noofbytes<0 || noofbytes>SIZE)
	{
		perror("th1 fail to write\n");
		printf("%d\n",errno);
		free(ptr);
		pthread_exit(ptr);
	}else
	{
		printf("data writen %s\n",ptr->buffer);
	}
	fl *mptrth=calloc(0,sizeof(fl));
	memcpy(mptrth,ptr,sizeof(fl));
	printf("th_fun:addrss:%p fp:%d\n",mptrth,mptrth->fp);
	free(ptr);
	pthread_exit((void *)mptrth);
	return mptrth;
}

static void *th_fun1(void *arg){
	fl *pptr=(fl *)arg;
	int noofbytes=0;
	printf("th_fun1:addrss:%p data:%d\n",pptr,pptr->arg);
	snprintf(pptr->buffer,SIZE,"2:%d",pptr->arg);
	noofbytes=write(pptr->fp,pptr->buffer,SIZE);
	if(noofbytes<0 || noofbytes>SIZE)
	{
		perror("write fail\n");
		printf("%d",errno);
		exit(0);
	}else{
		printf("da:%s sz:%ld\n",pptr->buffer,sizeof(pptr->buffer));
	}
	free(pptr);
	fl *mptrth1=calloc(0,sizeof(fl));
	printf("th_fun:addrss:%p\n",mptrth1);
	pthread_exit((void *)mptrth1);
	return mptrth1;
}

void th_crt(pthread_t *t1,void *(*fp)(void *arg),void *arg){
	fl *ptr_arg=(fl *)arg,*mptr_arg;
	int chk;
	pthread_attr_t atr;
	pthread_attr_init(&atr);
	pthread_attr_setdetachstate(&atr, PTHREAD_CREATE_JOINABLE);
	mptr_arg=calloc(0,sizeof(fl));
	if(!(mptr_arg))
	{
		perror("memory allocation failed\n");
		printf("%d\n",errno);
		exit(1);
	}
	memcpy(mptr_arg,ptr_arg,sizeof(*mptr_arg));
	printf("th_crt:mem address:%p fp:%d\n",mptr_arg,mptr_arg->fp);
	chk=pthread_create(t1, &atr, fp, (void *)mptr_arg);
	if(chk!=0)
	{
		perror("pthread create fail\n");
		printf("%d",errno);
		free(mptr_arg);
		exit(0);
	}
	pthread_attr_destroy(&atr);
}


int main()
{
	pthread_t t[NOTH];
	void *(*arr_fn[NOTH])(void *arg)={th_fun,th_fun1};
	fl a[NOTH],*file[NOTH];
	int i=0,chk=0;

	while(i<NOTH)
	{
		memset(&a[i],0,sizeof(fl));
		file[i]=(a+i);
		file[i]->arg=i+10;
		file[i]->fp=open("./tst.txt",O_CREAT|O_APPEND|O_RDWR|O_NONBLOCK);
		if(!(file[i]->fp))
		{
			perror("file is not open\n");
			printf("%d",errno);
			exit(0);
			return EXIT_FAILURE;
		}
		th_crt((t+i),*(arr_fn+i),*(file+i));
		fl *ptr_arr;
		chk=pthread_join(t[i],(void **)&ptr_arr);
		if(chk!=0)
		{
			perror("pthread join\n");
			return EXIT_FAILURE;
		}
		fl *pptr=(fl *)ptr_arr;
		printf("addrss:%p fp:%d\n",pptr,pptr->fp);
		close(pptr->fp);
		free(pptr);
		i++;
	}
	return 0;
}
#endif
