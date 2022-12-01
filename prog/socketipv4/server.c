/*
 * server.c
 *
 *  Created on: Nov 30, 2022
 *      Author: ubuntu
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define e(a) printf("%d\n",a)

int main()
{

	struct sockaddr_in server_addr,client_addr;
	int server_fd=-1,soc_opt=-1, flag=0,ret=-1,digit=0,nw_acp=-1,noofbyte=0;
	socklen_t opt_sz,server_sz,client_sz;
	//socket
	server_fd=socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd<0)
	{
		perror("socket failed\n");
		e(server_fd);
		return EXIT_FAILURE;
	}

	//setsockopt reuse addr
	flag=1;
	opt_sz=sizeof(flag);
	soc_opt=setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT	, &flag,opt_sz);
	if(soc_opt<0)
	{
		perror("socket option fail\n");
		e(soc_opt);
		return EXIT_FAILURE;
	}

	//bind
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=ntohl(INADDR_LOOPBACK); //study loop back
	server_addr.sin_port=htons(2000);
	server_sz=sizeof(server_addr);
	ret=bind(server_fd,(struct sockaddr*)&server_addr, server_sz);
	if(ret<0)
	{
		perror("bind failed addressing\n");
		e(ret);
		return EXIT_FAILURE;
	}
	//listen

	ret=listen(server_fd, 5);
	if(ret<0)
	{
		perror("listen fail\n");
		e(ret);
		return EXIT_FAILURE;
	}

	while(1)
	{
	//accept
		client_sz=sizeof(client_addr);
		nw_acp=accept(server_fd, (struct sockaddr*)&client_addr, &client_sz);
		if(nw_acp<0)
		{
			perror("accept fail\n");
			e(ret);
			return EXIT_FAILURE;
		}

	//send
		noofbyte=sendto(nw_acp, &digit, sizeof(digit), flag, (struct sockaddr*)&client_addr, client_sz);
		if(noofbyte==0)
		{
			perror("failed to send\n");
			e(noofbyte);
			return EXIT_FAILURE;
		}
	//recv
		digit++;

		noofbyte=recv(nw_acp, &digit, sizeof(digit), flag);
		if(noofbyte==0)
		{
			perror("recveded msg failed\n");
			e(noofbyte);
			return EXIT_FAILURE;
		}

	}
	return 0;
}
