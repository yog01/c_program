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

	shmid=shmget((key_t)1234,1024,0666|IPC_CREAT);

	printf("shmid %d\n",shmid);

	shared_memory=shmat(shmid, NULL, 0);

	printf("Data read %s\n",(char *)shared_memory);




	return 0;
}
