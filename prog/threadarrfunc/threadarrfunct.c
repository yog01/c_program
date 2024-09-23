/*
 * threadarrfunct.c
 *
 *  Created on: 05-Mar-2024
 *      Author: yo
 */
/*
 * thread_.c
 *
 *  Created on: 05-Mar-2024
 *      Author: yo
 */
#define _UNIX03_THREADS
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#define NOOFTHREAD 2
#define NOOFFUN 2

extern int errno;

pthread_t thread_id[NOOFTHREAD];

int th_crt();

static void *a_dd(void *arg){
	int a=*(int *)arg,sum=0,*ptr_sum;
	sum=a+10;
	printf("in addfun address %p sum %d\n",arg,sum);
	ptr_sum=(int *)calloc(0,sizeof(int));
	*ptr_sum=sum;
	printf("in addfun 2nd memory address %p\n",ptr_sum);
	pthread_exit((void *)ptr_sum);
}
static void *su_b(void *arg){
	int b=*(int *)arg,sub=0,*ptr_sub,b1;
	free((void *)arg); //move free on next function
	printf("Enter 2nd no to subtract\n");
	scanf("%d",&b1);
	sub=b-b1;
	printf("in sub address %p sub %d\n",arg,sub);
	ptr_sub=(int *)calloc(0,sizeof(int));
	*ptr_sub=sub;
	printf("in subtract function 3rd memory address %p\n",ptr_sub);
	pthread_exit((void *)ptr_sub);
}
int main(){

	int chk=0,*ca,i=0,*ptr_ca;
	chk=th_crt();
	printf("th_crt %d\n",chk);
	printf("waitng for thread to join\n");
	for(i=0;i<NOOFTHREAD;i++){
		chk=pthread_join(thread_id[i],(void **)&ca);
		if(chk!=0)
		{
			perror("phtread_join fail");
			printf("pthread_join fail thread id %d error%d",i,errno);
			exit(EXIT_FAILURE);
		}
		ptr_ca=(int *)ca;
		printf("in main add func join chk%d address %p\n",*ptr_ca,ptr_ca);
		free(ptr_ca);
	}

	return 0;
}

int th_crt(){
	int i=0,ds=0,chk=0,*arg;
	pthread_attr_t atr;
	void *(*arr_fn[NOOFFUN])(void *)={a_dd,su_b};
	chk=pthread_attr_init(&atr);
	if(chk==-1)
	{
		perror("phtread_attr_init fail");
		exit(EXIT_FAILURE);
	}
	chk=pthread_attr_setdetachstate(&atr,ds);
	if(chk==-1){
		perror("phtread_attr_setdetachstate fail");
		exit(EXIT_FAILURE);
	}
	int sz=sizeof(arr_fn)/sizeof(arr_fn[0]);
	printf("sz of arr of fn pointer %d\n",sz);

	arg=(int *)calloc(0,sizeof(int));
	*arg=10;
	for(i=0;i<NOOFTHREAD;i++)
	{
		chk=pthread_create((thread_id+i), &atr,arr_fn[i],(void *)arg);
		if(chk==-1)
		{
			perror("phtread_create fail");
			printf("thread id %d error%d",i,errno);
			exit(EXIT_FAILURE);
		}
	}
	return chk;
}
