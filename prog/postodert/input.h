/*
 * input.h
 *
 *  Created on: 23-Nov-2024
 *      Author: yo
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define NUMBER '0'
#define BUFSIZE 100


int _getch(void);
void ungetch(int a);
int getopt(char s[]);



#endif /* INPUT_H_ */
