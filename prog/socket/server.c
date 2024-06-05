/*
 * server.cpp
 *
 *  Created on: 19 Jun 2021
 *      Author: yo
 */

#include <socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pipe.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	struct sockaddr_in addr;
	int so_fd,nw_fd,opt=1;

	so_fd=socket(AF_INET,SOCK_STREAM,0);
	if(so_fd<1)
	{
		perror("socket fail\n");
		exit(EXIT_FAILURE);
	}

	setsockopt(so_fd,,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));

	return 0;
}
