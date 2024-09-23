/*
 * threadfilelog.h
 *
 *  Created on: 25-Apr-2024
 *      Author: yo
 */

#ifndef THREADFILELOG_H_
#define THREADFILELOG_H_
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
extern int errno;
#define SIZE 2
#define SIZEE 64
#define FLSZ 1024
static void *th_add(void *arg);
static void *th_sub(void *arg);
static void fun_time(void *_ptrdata);
static void creat_file(void *ptrdata);

#endif /* THREADFILELOG_H_ */
