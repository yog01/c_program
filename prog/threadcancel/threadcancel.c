/*
 * threadfilenm.c
 *
 *  Created on: 13-May-2024
 *      Author: yo
 */

#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 2
#define SIZEFL 25
#define flnmsz 10
typedef struct da{
	int fp,id;
}data;

void th_crt(pthread_t *t1,void *_da,void *(*fp)(void *arg))
{
	int chk=0;
	data *da=(data *)_da,*mptr;
	pthread_attr_t atr;
	mptr=(data *)calloc(0,sizeof(data));
	memcpy(mptr,da,sizeof(data));
	printf("in th_crt():file fp:%d",mptr->fp);
	pthread_attr_init(&atr);
	pthread_attr_setdetachstate(&atr, PTHREAD_CREATE_JOINABLE);
	chk=pthread_create(t1, &atr, fp, mptr);
	if(chk!=0)
	{
		perror("thread create fail\n");
		printf("%d",errno);
		free(da);
		free(mptr);
		exit(0);
	}
	pthread_attr_destroy(&atr);
}
static void *th_fun1(void *_arg)
{
	data *arg=(data *)_arg;
	int noofbyt=0;
	char flda[SIZEFL];
	snprintf(flda,SIZEFL,"./thread_%d.txt",arg->id);
	arg->fp=open(flda,O_CREAT|O_RDWR,0777);
	if(arg->fp<0)
	{
		perror("erro in open file\n");
		printf("%d",errno);
		exit(0);
	}else{
		printf("file pointer %d flnm:%s\n",arg->fp,flda);
	}
	while(1)
	{
		snprintf(flda,SIZEFL,"filefp:%d threadid:%d\n",arg->fp,arg->id);
		noofbyt=write(arg->fp,flda,SIZEFL);
		if(noofbyt<0)
		{
			perror("write fail\n");
			printf("%d",errno);
			exit(0);
		}
		sleep(5);
	}
	free(arg);
	close(arg->fp);
	pthread_exit(arg);
}
static void *th_fun2(void *_arg)
{
	data *arg=(data *)_arg;
	printf("filefp:%p %d\n",&arg->fp,arg->fp);

	free(arg);
	pthread_exit(arg);
}

int main()
{
	int chk=0;
	pthread_t th[SIZE];
	void *(*arr_fun[SIZE])(void *_arg)={th_fun1,th_fun2};
	data da1,*ptr_da=&da1;
	void *ptrret;
	int i=0,a=0;

	while(i<SIZE){
		ptr_da->id++;
		th_crt(th+i,ptr_da,arr_fun[0]); //arr_fun[i]
		printf("1.cancel thread%d\n",ptr_da->id);
		scanf("%d",&a);
		switch(a)
		{
			case 1:
				exit(0);
				break;
			default:
				continue;
		}
		chk=pthread_join(th[i], (void *)&ptrret);
		if(chk!=0)
		{
			perror("thread join fail\n");
			printf("%d",errno);
			exit(0);
		}
		data *ptrrret=(data *)ptrret;
		printf("ret ptr %p\n",ptrrret);
		close(ptr_da->fp);
		i++;
	}

	return 0;
}
