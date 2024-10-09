/*
 * thredcleanup.c
 *
 *  Created on: 08-Oct-2024
 *      Author: yo
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#define TH 2
#define NM 50

pthread_t pth[TH];


void mem_clean(void *arg)
{
	printf("%s invloked \n",__FUNCTION__);
	free(arg);
}

void fl_cln(void *arg){
	FILE *fp=(FILE *)arg;
	printf("%s invoked \n",__FUNCTION__);
	fclose(fp);
}

void *write_file(void *arg){

	FILE *fp;
	char filnm[NM],str_fl[NM];
	int *id=(int *)arg,count,noofby;
	sprintf(filnm,"thread%d.txt",*id);

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);

	pthread_cleanup_push(mem_clean,arg);



	fp=fopen(filnm,"a+");
	if(fp==NULL)
	{
		perror("file iss not open\n");
		pthread_exit(0);
	}

	pthread_cleanup_push(fl_cln,fp);

	while(1){
		noofby=sprintf(str_fl,"%d: I am thread %d\n",++count,*id);
		fwrite(str_fl,sizeof(char),noofby,fp);
		fflush(fp);
		sleep(1);
		pthread_testcancel();

	}

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	return 0;
}



int main(){
	int *mptr,i=0,chk;

	for(i=0;i<TH;++i)
	{
		mptr=(int *)calloc(1,sizeof(int));
		*mptr=i;
		chk=pthread_create(pth+i, NULL, write_file, mptr);
		if(chk!=0)
		{
			perror("thread create fail\n");
			return EXIT_FAILURE;
		}
	}

	int choice,thread_num;

	while(1){
		printf("1.Cacel thread\n");
		scanf("%d",&choice);

		switch (choice) {
			case 1:
				printf("enter the thread number[0-%d]\n",TH-1);
				scanf("%d",&thread_num);
				if(thread_num<0 || thread_num>=TH)
				{
					perror("invalid thread\n");
					return EXIT_FAILURE;
				}
				pthread_cancel(pth[thread_num]);
				break;
			default:
				continue;
		}

	}

	return 0;
}
