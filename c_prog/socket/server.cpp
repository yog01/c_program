/*
 * server.cpp
 *
 *  Created on: 19 Jun 2021
 *      Author: yo
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main()
{
	int sockfd,option_value=1;
	struct sockaddr_in address;
	//struct sockaddr *addr=NULL;
	socklen_t length=sizeof(address);
	//socklen_t length=sizeof(address);
	int new_socket;
	char buffer[1024]={0};
	char sendbuffer[20]="Hi from server";

	//socket file discriptor local address connect
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		perror("\nsocket file discriptor failed");
		return -1;
	}
	//socket
	if(setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE|SO_REUSEADDR,&option_value,sizeof(option_value))<0)
	{
		perror("\n socket creation failed");
		return -1;
	}
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=htonl(INADDR_ANY);
	address.sin_port=htons(PORT);
	//addr->sa_family=INADDR_ANY; //any address IPv4
	//bind it will assign address for newly created socket
	if(bind(sockfd,(struct sockaddr *)&address,length)<0)
	{
		perror("bind failed");
		return -1;
	}

	//listen
	if(listen(sockfd,3)<0)
	{
		perror("listen failed");
		return -1;
	}

	//accept
	new_socket=accept(sockfd,(struct sockaddr *)&address,&length);
	if(new_socket<0)
	{
		perror("accept failedd");
		return -1;
	}
	//send buffer
	printf("\nrecv msg to client");
	recv(new_socket,buffer,sizeof(buffer),MSG_CONFIRM);
	printf("msg:%s",buffer);
	memset(buffer,0,sizeof(buffer));
	printf("\nSending msg to client");
	memcpy(buffer,sendbuffer,sizeof(sendbuffer));
	send(new_socket,buffer,sizeof(buffer),MSG_CONFIRM);


 return 0;
}
