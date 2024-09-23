/*
 * client.c
 *
 *  Created on: 24-Jun-2023
 *      Author: yo
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
//#define client
#ifdef client
int main(int argc, char **argv) {

	struct sockaddr_in *ptr_addr=NULL;
	char buffer[INET_ADDRSTRLEN]={'\0'},snd[INET_ADDRSTRLEN]={"127.0.0.1"},recv[INET_ADDRSTRLEN]={'\0'},localhost[INET_ADDRSTRLEN]={"localhost"};
	int sfd,ret,opt=1,port=2001,noofbytes=0;
	socklen_t szaddr;
	ptr_addr=(struct sockaddr_in *)calloc(0,sizeof(*ptr_addr));

	sfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sfd<0)
	{
		perror("socket fail\n");
		printf("%d",errno);
		exit(0);
	}

	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_KEEPALIVE,&opt,sizeof(opt));
	if(ret<0)
	{
		perror("set sock opt fail\n");
		printf("%d",errno);
		exit(0);
	}
	ptr_addr->sin_family=AF_INET;
	ptr_addr->sin_port=htons(port);
	ptr_addr->sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	//inet_pton(AF_INET, localhost, &(*ptr_addr).sin_addr.s_addr);
	inet_ntop(AF_INET,&(*ptr_addr).sin_addr.s_addr,buffer,sizeof(buffer));
	printf("address client%s\n",buffer);
	szaddr=sizeof(*ptr_addr);
	ret=connect(sfd,(struct sockaddr*)ptr_addr,szaddr);
	if(ret<0)
	{
		perror("connect fail\n");
		printf("%d",errno);
		exit(0);
	}
	memcpy(buffer,snd,INET_ADDRSTRLEN);
	noofbytes=sendto(sfd,buffer,sizeof(buffer),0,(struct sockaddr *)ptr_addr,szaddr);
	if(noofbytes<0)
	{
		perror("sendto from fail\n");
		printf("%d",errno);
		exit(0);
	}
	printf("no of bytes %d snd %s\n",noofbytes,snd);
	memset(buffer,'\0',INET_ADDRSTRLEN);
	szaddr=sizeof(*ptr_addr);
	noofbytes=recvfrom(sfd,buffer,sizeof(buffer),0,(struct sockaddr *)ptr_addr,&szaddr);
	if(noofbytes<0)
	{
		perror("recv from fail\n");
		printf("%d",errno);
		exit(0);
	}
	printf("no of bytes %d recv %s",noofbytes,buffer);
	memcpy(recv,buffer,noofbytes);
	close(sfd);


	free(ptr_addr);
	return 0;
}
#endif
