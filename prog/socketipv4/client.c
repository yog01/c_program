/*
 * clieent.c
 *
 *  Created on: Dec 4, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>


#define e(a) printf("%d\n",a);

//#define client 1
#ifndef client
int main()
{
	int file_desp=-1,ret=0,flag=0,nwf_des=0,digit=1,noofbytes,por_t=2000;
	char port[BUFSIZ]={"2000"};
	struct sockaddr_in client_addr;
	socklen_t sz_client,sz_server;
	struct sockaddr server_addr;
	struct addrinfo *hints,*addrs;

	memset(hints,0,sizeof(struct addrinfo));
	hints->ai_family=AF_INET;
	hints->ai_flags=AI_PASSIVE;
	hints->ai_socktype=SOCK_STREAM;
	ret=getaddrinfo(NULL,port,hints,&addrs);
	if(0>ret)
	{
		perror("get addr info fail");
		e(ret);
		return EXIT_FAILURE;
	}
	printf("%d",addrs->ai_family);
	//socket

	if(0>(file_desp=socket(addrs->ai_family, addrs->ai_socktype, 0)))
	{
		perror("socket fail\n");
		e(file_desp);
		return EXIT_FAILURE;
	}

	//connect
	client_addr.sin_family=AF_INET;
	client_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK); //local host ip 127.0.0.1
	client_addr.sin_port=htons(por_t);
	sz_client=sizeof(client_addr);
	nwf_des=connect(file_desp, (struct sockaddr *)&client_addr,sz_client);
	if(nwf_des<0)
	{
			perror("connect fail\n");
			e(nwf_des);
			return EXIT_FAILURE;
	}

	//send
	sz_server=sizeof(server_addr);
	noofbytes=sendto(nwf_des, &digit,sizeof(digit),flag,(struct sockaddr *)&server_addr,sz_server);
	if(noofbytes<0)
	{
		perror("connect fail\n");
		e(noofbytes);
		return EXIT_FAILURE;
	}
	printf("no of bytes\n");
	e(noofbytes);
	//recv
	noofbytes=recv(nwf_des, &digit, sizeof(digit), flag);
	if(noofbytes<0)
	{
		perror("connect fail\n");
		e(noofbytes);
		return EXIT_FAILURE;
	}
	printf("no of bytes\n");
	e(noofbytes);
	printf("recived digit\n");
	e(digit);

	return 0;
}
#endif