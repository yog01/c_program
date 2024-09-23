/*
 * threadfilelog.c
 *
 *  Created on: 24-Apr-2024
 *      Author: yo
 */
#include "threadfilelog2th.h"

#define twothread
#ifndef twothread
#define FLPATH "./timelog.txt"

typedef struct{
	time_t msec,sec,min,hr;
	char buffer[SIZEE];
	//FILE *fp;
	int data,fp;
}cal;

pthread_mutex_t thsyn;
void fun_time(void *_usec,void *_sec,void *_min,void *_hr)
{

	time_t *msec=(time_t *)_usec,*sec=(time_t *)_sec,*min=(time_t *)_min,*hr=(time_t *)_hr;
	printf("in fun_time:usec%ld\n",*msec);
	pthread_mutex_lock(&thsyn);
	*msec=*msec/1000;
	*sec=*msec/1000;
	while(*sec>1)
	{
		if(3600>*sec && *sec>60)
		{
			*min=*sec;
		}else if(*min>60)
		{
			*hr=*sec;
			break;
		}
		*sec=*sec/60;
	}
	*sec=*msec/1000;
	*msec=*msec%1000;
	pthread_mutex_unlock(&thsyn);
	return;
}

static void *th_add(void *arg){
	cal *ptra=(cal *)arg;
	int noofbytess,n;
	fun_time(&ptra->msec, &ptra->sec, &ptra->min, &ptra->hr);
	ptra->data=1;
	printf("in th_add function:msec%ld sec%ld min%ld hr%ld data:%d\n",ptra->msec,ptra->sec,ptra->min,ptra->hr,ptra->data);
	noofbytess=snprintf((char *)ptra->buffer,SIZEE,"msec:%ld sec:%ld min:%ld hr:%ld data:%d\n\r",(time_t)ptra->msec,(time_t)ptra->sec,(time_t)ptra->min,(time_t)ptra->hr,(int)ptra->data);
	//n=fwrite((char *)ptra->buffer,SIZEE,1,ptra->fp);
	n=write(ptra->fp,ptra->buffer,SIZEE);
	if(n<0 || n>SIZEE)
	{
		perror("fwrite fail\n");
		exit(EXIT_FAILURE);
	}else{
		printf("msec:%ld noofby %d\n",ptra->msec,noofbytess);
	}
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
	int noofbytes;
	cal *ptrs=(cal *)arg;
	fun_time(&ptrs->msec, &ptrs->sec, &ptrs->min, &ptrs->hr);
	ptrs->data=2;
	printf("msec:%ld sec:%ld min:%ld hr:%ld data:%d\n",ptrs->msec,ptrs->sec,ptrs->min,ptrs->hr,ptrs->data);
	noofbytes=snprintf((char *)ptrs->buffer,SIZEE,"2: msec%ld sec%ld min%ld hr%ld data:%d\n\r",(time_t)ptrs->msec,(time_t)ptrs->sec,(time_t)ptrs->min,(time_t)ptrs->hr,(int)ptrs->data);
	if(noofbytes<0 || noofbytes>SIZEE)
	{
		perror("buffer size is less\n");
		exit(EXIT_FAILURE);
	}else{
		printf("sz %d\n",noofbytes);
	}
	//noofbytes=fwrite(ptrs->buffer,SIZEE,1,ptrs->fp);
	noofbytes=write(ptrs->fp,ptrs->buffer,SIZEE);
	if(noofbytes<0 || noofbytes>SIZEE)
	{
		perror("fwrite fail\n");
		printf("%d",errno);
		exit(EXIT_FAILURE);
	}else{
		printf("in th_sub function msec%ld sec%ld min%ld hr%ld address %p data:%d\n",ptrs->msec,ptrs->sec,ptrs->min,ptrs->hr,ptrs,ptrs->data);
	}
	cal *tptrs=(cal *)malloc(sizeof(cal));
	if(!tptrs)
	{
		perror("meory not created\n");
		printf("%d",errno);
		exit(EXIT_FAILURE);
	}
	memcpy(tptrs,ptrs,sizeof(*ptrs));
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

	int i=0,j=1,chk=0;
	cal **ret=NULL, time_da[SIZE];
	void *(*arr[SIZE])(void *arg)={th_add,th_sub};
	pthread_t t1[SIZE];
	clock_t usec;
	pthread_mutex_init(&thsyn, NULL);
	memset(time_da,0,SIZE*sizeof(cal));
	while(i<SIZE)
	{
		if(!(time_da[i].fp=open(FLPATH,O_CREAT|O_APPEND|O_RDWR|O_NONBLOCK)))
		{
			perror("file not created\n");
			printf("%d\n",errno);
			exit(EXIT_FAILURE);
		}
		while(j<=4) //61 1min 1 sec
		{
			usec=clock();
			while(clock()<usec+1000000);
			j++;
		}
		time_da[i].msec=usec;
		time_da[i].sec=0;
		time_da[i].min=0;
		time_da[i].hr=0;
		j=1;
		th_crt(t1+i,&time_da[i],*(arr+i));
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
		close(time_da[i].fp);
		i++;
	}
	pthread_mutex_destroy(&thsyn);

	return 0;
}
#endif
