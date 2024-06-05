#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define PORT 8080
#define clie
#ifndef clie
int main()
{

    //struct sockaddr *addr = NULL;
    int sockfd;
    struct sockaddr_in address;
	socklen_t leng=sizeof(address);
	char buffer[1024]={0};
	char sendbuffer[20]="Hi from client";
	//socket
	sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
    	perror("\n socket failed");
    	return -1;
    }
	//connect
    //addr->sa_family=AF_INET;
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=htonl(INADDR_ANY);
    address.sin_port=htons(PORT);

    if(connect(sockfd,(struct sockaddr *)&address,leng)<0)
    {
    	perror("\n socket failed");
    	return -1;
    }

	memcpy(buffer,sendbuffer,sizeof(sendbuffer));
	send(sockfd,buffer,sizeof(buffer),MSG_CONFIRM);
	printf("\nSend msg to server");
	memset(buffer,0,sizeof(buffer));
	printf("\nrecv msg to server");
	recv(sockfd,buffer,sizeof(buffer),MSG_CONFIRM);
	printf("msg:%s",buffer);


	return 0;
}
#endif
