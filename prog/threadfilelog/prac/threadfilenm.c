/*
 * threadfilenm.c
 *
 *  Created on: 13-May-2024
 *      Author: yo
 */
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define thfilenm
#ifndef thfilenm
#define SIZE 30+34 //32
#define SZDA 1024 //1Kb=1024 bytes
#define TH 2
#define FLNM(stflnm,t,ut) snprintf(stflnm,SIZE,"./%d_%d_%d:%d:%d:%d:%ld.txt",t->tm_mday,t->tm_mon+1,t->tm_year+1900,t->tm_hour,t->tm_min,t->tm_sec,ut.tv_usec)

pthread_mutex_t mut =PTHREAD_MUTEX_INITIALIZER;

extern int errno;

typedef struct{
	char flnm[SIZE],buffer[SIZE];
	time_t ti;
	size_t bytsz;
	struct tm *curtm;
	struct timeval tp;
}stda;

void get_time__(void *pdata)
{
	stda *ptrdata=(stda *)pdata;
	int chk=0;
	pthread_mutex_lock(&mut);
	ptrdata->ti=time(NULL);
	chk=gettimeofday(&ptrdata->tp,NULL);
	if(chk<0)
	{
		perror("get time of day fail\n");
		printf("%d",errno);
		exit(0);
	}
	ptrdata->curtm=localtime(&ptrdata->ti);
	memset(ptrdata->buffer,0,SIZE);
	snprintf(ptrdata->buffer,SIZE,"hr%d min%d sec%d usec%ld\r\n",ptrdata->curtm->tm_hour,ptrdata->curtm->tm_min,ptrdata->curtm->tm_sec,ptrdata->tp.tv_usec);
	pthread_mutex_unlock(&mut);
	return ;
}
void create_file(stda *_ptrdata)
{

	int nobyte,fp;
	get_time__(_ptrdata);
	FLNM(_ptrdata->flnm,_ptrdata->curtm,_ptrdata->tp);
	fp=open(_ptrdata->flnm,O_CREAT|O_RDWR,0777);
	if(fp<0)
	{
		perror("file is not open\n");
		printf("%d\n",errno);
		pthread_exit((void *)_ptrdata);
	}else{
		printf("file is created file %d flnm %s\n",fp,_ptrdata->flnm);
	}
	_ptrdata->bytsz=0;
	while(_ptrdata->bytsz<SZDA)
	{

		if(_ptrdata->bytsz>=0) //64*16=1024 By or 1kB
		{
			get_time__(_ptrdata);
			_ptrdata->bytsz+=write(fp,_ptrdata->buffer,SIZE);
			//printf("in th1:actual data written %ld da %s\n",_ptrdata->bytsz,_ptrdata->flnm);

		}else{
			perror("write fail\n");
			printf("%d\n",errno);
			pthread_exit((void *)_ptrdata);
		}

	}
	lseek(fp,0,SEEK_SET);
	nobyte=0;
	while(nobyte<SZDA)
	{
		nobyte+=read(fp,_ptrdata->buffer,SIZE);
		if(nobyte<0)
		{
			perror("read fail\n");
			printf("%d",errno);
			exit(0);
		}else{
			printf("sz:%d da:%s\n",nobyte,_ptrdata->buffer);
		}
	}
	close(fp);
}
static void *th_fun1(void *arg){
	stda *ptrdata=(stda *)arg;

	create_file(ptrdata);
	create_file(ptrdata);

	stda *mptr=(stda *)malloc(sizeof(stda));
	free(ptrdata);
	pthread_exit((void *)mptr);
}
static void *th_fun2(void *arg){
	stda *ptrdata=(stda *)arg;
	get_time__(ptrdata);
	printf("in th_fun2: day %d mon %d yr %d\n",ptrdata->curtm->tm_mday,ptrdata->curtm->tm_mon+1,ptrdata->curtm->tm_year+1900);
	stda *mptr=(stda *)malloc(sizeof(stda));
	free(ptrdata);
	pthread_exit((void *)mptr);
}


int main(){
	pthread_t t1[TH];
	int i=0,chk,*ptrr;
	pthread_attr_t atr;
	stda *data[TH];
	void *(*arr_fun[TH])(void *arg)={th_fun1,th_fun2};
	pthread_mutex_init(&mut, NULL);
	pthread_attr_init(&atr);
	pthread_attr_setdetachstate(&atr, PTHREAD_CREATE_JOINABLE);
	while(i<TH)
	{
		data[i]=(stda *)malloc(sizeof(stda));
		chk=pthread_create(&t1[i], &atr,arr_fun[i], (void *)data[i]);
		if(chk!=0)
		{
			perror("pthread create fail\n");
			printf("%d",errno);
			exit(0);
		}
		sleep(1);

		chk=pthread_join(t1[i],(void **)&ptrr);
		if(chk!=0)
		{
			perror("fail to open file\n");
			printf("%d %d",errno,i);
			exit(0);
		}
		int *pptr=(int *)ptrr;
		printf("address %p\n",pptr);
		free(pptr);
		i++;
	}
	pthread_mutex_destroy(&mut);
	pthread_attr_destroy(&atr);
	return 0;
}
#endif//thfilenm
