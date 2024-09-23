/*
 * hello_world.c
 *
 *  Created on: 04-Nov-2023
 *      Author: yo
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static void *th_fn(void *arg){
	char *ptr_arg=(char *)arg;
	while(1)
	{
		printf("arg: %s\n",ptr_arg);
		sleep(1);
	}
}

void _pthread_create(){
	pthread_t thread;

	static char *arg="thread1";

	int chk=pthread_create(&thread, NULL, th_fn,(void *)arg);
	if(chk!=0)
	{
		printf("thread create fail\n");
		exit(0);
	}
}

int main(){
	_pthread_create();
	printf("main fn pause\n");
	//sleep(2);
	//pause();
	pthread_exit(0); //it will not terminate threads exit(0) will terminates all threads
	return 0;
}

