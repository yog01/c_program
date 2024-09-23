/*
 * arr2ddynamic.c
 *
 *  Created on: 17-Mar-2024
 *      Author: yo
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 2
#define STRSZ 5
void arrfunc(void *ptr)
{
    char *tptr=(char *)ptr,*temp;
    temp=(char *)calloc(0,STRSZ*sizeof(char));
    memcpy(temp,tptr,SIZE);
    printf("in func %s\n",temp);
    free(temp);
}

int main()
{
    char *arr[SIZE];
    int i=0;
    while(i<SIZE)
    {
        arr[i]=calloc(0,STRSZ*sizeof(char));
        i++;
    }
    memcpy(arr[0],"t1",STRSZ);
    memcpy(arr[1],"t2",STRSZ);

    i=0;
    while(i<SIZE)
    {
        arrfunc((void *)arr[i]);
        free(arr[i]);
        i++;
    }
    return 0;
}
