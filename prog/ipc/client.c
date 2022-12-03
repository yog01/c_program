/*
 * server.c
 *
 *  Created on: Mar 24, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#define size 100
int main()
{
	int shmid;
	void *shared_memory;
	char buff[size];
	shmid=shmget((key_t)1234, 1024, 0666|IPC_CREAT);

	printf("shared memory id %u\n",shmid);

	shared_memory=shmat(shmid,NULL,0);

	printf("Process attach at shared memory%p\n",shared_memory);

	printf("Enter the data\n");

	scanf("%s",buff);

	strcpy(shared_memory,buff);

	printf("Data writen %s\n",(char *)shared_memory);

	return 0;
}
