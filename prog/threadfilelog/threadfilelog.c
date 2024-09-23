/*
 * threadfilelog.c
 *
 *  Created on: 24-Apr-2024
 *      Author: yo
 */
#include "threadfilelog.h"

#define FLPATH(flnm,ptrcal) snprintf(flnm,SIZEE,"./%d_%d_%d:%ld:%ld:%ld:%ld.txt",ptrcal->_day->tm_mday,ptrcal->_day->tm_mon+1,ptrcal->_day->tm_year+1900,ptrcal->hr,ptrcal->min,ptrcal->sec,ptrcal->msec)

typedef struct{
	time_t msec,sec,min,hr,t;
	char buffer[SIZEE];
	size_t noofbytes;
	char flnm[SIZEE];
	struct tm *_day;
}cal;

pthread_mutex_t thsyn;
static void fun_time(void *_ptrdata)
{

	cal *__ptrdata=(cal *)_ptrdata;
	int j=0;
	pthread_mutex_lock(&thsyn);
	while(j<4) //4sec 61 1min 1 sec
	{
		__ptrdata->msec=clock();
		while(clock()<(__ptrdata->msec+1000000));
		j++;
	}
	printf("in fun_time:usec%ld\n",__ptrdata->msec);
	__ptrdata->t=time(NULL);
	__ptrdata->_day=localtime(&__ptrdata->t);

	__ptrdata->msec=__ptrdata->msec/1000;
	__ptrdata->sec=__ptrdata->msec/1000;
	while(__ptrdata->sec>1)
	{
		if(3600>__ptrdata->sec && __ptrdata->sec>60)
		{
			__ptrdata->min=(__ptrdata->sec)/60;
		}else if(__ptrdata->min>60)
		{
			__ptrdata->hr=__ptrdata->sec;
			break;
		}
		__ptrdata->sec=__ptrdata->sec/60;
	}
	__ptrdata->sec=(__ptrdata->msec/1000)%60;
	__ptrdata->msec=__ptrdata->msec%1000;
	printf("in fun time function:msec%ld sec%ld min%ld hr%ld\n",__ptrdata->msec,__ptrdata->sec,__ptrdata->min,__ptrdata->hr);
	snprintf((char *)__ptrdata->buffer,SIZEE,"msec:%ld sec:%ld min:%ld hr:%ld \n\r",(time_t)__ptrdata->msec,(time_t)__ptrdata->sec,(time_t)__ptrdata->min,(time_t)__ptrdata->hr);
	pthread_mutex_unlock(&thsyn);
	return;
}
static void creat_file(void *ptrdata)
{
		cal *ptra=(cal *)ptrdata;
		int fp;
		fun_time(ptra);
		FLPATH(ptra->flnm,ptra);
		printf("flnm %s\n",ptra->flnm);

		fp=open(ptra->flnm,O_CREAT|O_RDWR,0777);
		if(fp<0)
		{
			perror("file is not created\n");
			printf("%d",errno);
			exit(0);
		}else{
			printf("file is created %d\n",fp);
		}
		ptra->noofbytes=0;
		while(ptra->noofbytes<FLSZ){
			fun_time(ptra);
			ptra->noofbytes+=write(fp,ptra->buffer,SIZEE);
			if(ptra->noofbytes<0)
			{
				perror("write fail\n");
				printf("%d",errno);
				exit(EXIT_FAILURE);
			}else{
				printf("msec:%ld noofby %ld\n",ptra->msec,ptra->noofbytes);
			}
		}
		close(fp);

}

static void *th_add(void *arg){
	cal *ptra=(cal *)arg;
	creat_file(ptra);
	creat_file(ptra);
	cal *tmpprt=(cal *)malloc(sizeof(cal));
	if(!tmpprt)
	{
		perror("memory not created\n");
		exit(EXIT_FAILURE);
	}else{
		printf("in th_add function address: memory %p\n",tmpprt);
	}
	memcpy(tmpprt,ptra,sizeof(cal));
	pthread_exit((void *)tmpprt);
	free(ptra);
	ptra=NULL;
	return tmpprt;
}

static void *th_sub(void *arg){
	cal *ptrs=(cal *)arg;
	printf("address:%p msec:%ld sec:%ld min:%ld hr:%ld\n",ptrs,ptrs->msec,ptrs->sec,ptrs->min,ptrs->hr);
	cal *tptrs=(cal *)malloc(sizeof(cal));
	if(!tptrs)
	{
		perror("meory not created\n");
		printf("%d",errno);
		exit(EXIT_FAILURE);
	}
	pthread_exit((void *)tptrs);
	free((void *)ptrs);
	return tptrs;
}

void th_crt(pthread_t *t1,void *arg,void *(*fp)(void *arg)){
	int chk=0;
	cal *mptr=(cal *)malloc(sizeof(cal));
	if(!mptr)
	{
		perror("memory not created\n");
		exit(EXIT_FAILURE);
	}
	pthread_attr_t atr;
	pthread_attr_init(&atr);
	pthread_attr_setdetachstate(&atr, PTHREAD_CREATE_JOINABLE);
	memcpy(mptr,(cal *)arg,sizeof(cal));
	printf("in th_crt address:th_id %p da %p\n",t1,mptr);
	chk=pthread_create(t1, &atr, fp, (void *)mptr);
	if(chk!=0)
	{
		perror("thread not created\n");
		printf("%d",errno);
		free(mptr);
		exit(EXIT_FAILURE);
	}
	pthread_attr_destroy(&atr);
}

int main(){

	int i=0,chk=0;
	cal **ret=NULL, time_da[SIZE];
	void *(*arr[SIZE])(void *arg)={th_add,th_sub};
	pthread_t t1[SIZE];
	pthread_mutex_init(&thsyn, NULL);
	memset(time_da,0,SIZE*sizeof(cal));
	while(i<SIZE)
	{

		th_crt(t1+i,&time_da[i],*(arr+i)); //change to *(arr+i) to *(arr+0) for th_add
		cal a,*ptr=&a;
		ret=&ptr;
		chk=pthread_join(*(t1+i),(void **)ret);
		if(chk!=0)
		{
			perror("pthread_join fail\n");
			printf("%d",errno);
			exit(EXIT_FAILURE);
		}
		cal *pptr=*(cal **)ret;
		printf("return address %p\n",pptr);
		free(pptr);
		pptr=NULL;
		i++;
	}
	pthread_mutex_destroy(&thsyn);
	return 0;
}
