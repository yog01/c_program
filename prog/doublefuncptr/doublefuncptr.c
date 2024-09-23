/*
 * doublefuncptr.c
 *
 *  Created on: 09-May-2024
 *      Author: yo
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
extern int errno;
void *(*fp1)(void *arg);
void *(**fp)(void *arg);

void *add(void *arg)
{
	int *ptr=(int *)arg,*mptr;

	mptr=calloc(0,sizeof(int));
	if(!mptr)
	{
		perror("mem not created\n");
		printf("%d",errno);
		return NULL;
	}else{
		*mptr=*ptr;
		printf("mem addr %p da %d",mptr,*mptr);
	}
	return mptr;
}
int main(){
	int a=10,*found;
	fp1=add;
	fp=&fp1;
	printf("function %p\n",add);
	found=(**fp)(&a);
	printf("in main address:%p\n",found);
	free(found);
	return 0;
}
