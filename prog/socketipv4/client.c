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
	int file_desp=-1,ret=0,flag=0,nwf_des=0,digit=1,noofbytes,por_t=2111;
	char port[BUFSIZ]={"2111"},client_addr_chk[INET_ADDRSTRLEN]={'\0'},client_addr_buf[INET_ADDRSTRLEN]={"255.255.0.11"},hst_name[BUFSIZ]={"tacacs"},**ptr=NULL;
	struct sockaddr_in client_addr;
	socklen_t sz_client,sz_server;
	struct sockaddr server_addr;
	struct addrinfo *hints,*addrs,a_hints;
	struct servent *ptr_svc_info,srvc_info;
	hints=&a_hints;//Initalize
	ptr_svc_info=&srvc_info;
	//information

	memset(hints,0,sizeof(struct addrinfo));
	memset(ptr_svc_info, 0, sizeof(struct servent));
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

	ptr_svc_info=getservbyname(hst_name, "tcp"); //same as protocol and port service name
	if(!ptr_svc_info)
	{
		printf("not able to get services\n");
		return EXIT_FAILURE;
	}
	printf("----%s",hst_name);
	printf("local host srv name%s",ptr_svc_info->s_name);
	ptr=ptr_svc_info->s_aliases;
	while(*ptr)
	{
		printf("aliase name %s",*ptr);
		(*ptr)++;
	}


	//socket

	if(0>(file_desp=socket(addrs->ai_family, addrs->ai_protocol, 0)))
	{
		perror("socket fail\n");
		e(file_desp);
		close(file_desp);
		return EXIT_FAILURE;
	}
	memset(&client_addr, 0, sizeof(client_addr));
	//connect
	client_addr.sin_family=AF_INET;
	client_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK); //local host ip 127.0.0.1 //
	//inet_pton(AF_INET,client_addr_buf,&client_addr.sin_addr.s_addr);
	client_addr.sin_port=htons(por_t);
	sz_client=sizeof(client_addr);
	inet_ntop(AF_INET,&client_addr.sin_addr.s_addr, client_addr_chk, sizeof(client_addr_chk));
	printf("client adder %s addr sz%ld\n",client_addr_chk,sizeof(client_addr.sin_addr.s_addr));
	nwf_des=connect(file_desp, (struct sockaddr *)&client_addr,sz_client);
	if(nwf_des<0)
	{
			perror("connect fail\n");
			e(nwf_des);
			close(file_desp);
			return EXIT_FAILURE;
	}

	//send
	sz_server=sizeof(server_addr);
	noofbytes=sendto(nwf_des, &digit,sizeof(digit),flag,(struct sockaddr *)&server_addr,sz_server);
	if(noofbytes<0)
	{
		perror("send fail\n");
		e(noofbytes);
		close(nwf_des);
		return EXIT_FAILURE;
	}
	printf("no of bytes\n");
	e(noofbytes);
	//recv
	noofbytes=recv(nwf_des, &digit, sizeof(digit), flag);
	if(noofbytes<0)
	{
		perror("recv fail\n");
		e(noofbytes);
		close(nwf_des);
		return EXIT_FAILURE;
	}
	printf("no of bytes\n");
	e(noofbytes);
	printf("recived digit\n");
	e(digit);
	close(file_desp);
	close(nwf_des);
	return 0;
}
#endif
