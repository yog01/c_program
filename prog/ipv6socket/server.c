/*
 * server.c
 *
 *  Created on: Nov 27, 2022
 *      Author: ubuntu
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>


#define e(a) printf("error no:%d",a)
int main(int argc, char **argv) {

	int skt_fd=-1,port=2000,ret=0;
	struct sockaddr_in6 server_addr,client_addr;
	char host_nm[BUFSIZ]={'\0'},ch_port[BUFSIZ]={"2000"};

	struct addrinfo	*ptr_hint=NULL,*ptr_rslt=NULL;

	gethostname(host_nm, 255);
	printf("%s\n",host_nm);

	ptr_hint=calloc(1,sizeof(struct addrinfo));
	ptr_rslt=calloc(1,sizeof(struct addrinfo));

	ptr_hint->ai_family=PF_INET6;
	ptr_hint->ai_protocol=IPPROTO_IPV6;
	ptr_hint->ai_socktype=SOCK_STREAM; //SOCK_RAW fail for listen

	ret=getaddrinfo(host_nm, ch_port, ptr_hint, &ptr_rslt);
	if(ret<0)
	{
		perror("not able to get addr info\n");
		e(ret);
		return -1;
	}
	printf("%d",ptr_rslt->ai_family);

	//socket


	//setsockopt


	//bind


	//listen


	//accept


	// send recv


	return 0;
}

