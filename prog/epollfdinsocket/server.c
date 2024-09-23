/*
 * server.c
 *
 *  Created on: 01-Jul-2023
 *      Author: yo
 */
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
//#include <sys/poll.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <fcntl.h>
#define MAXFD 2
#define server
#ifdef server
int main()
{
	struct sockaddr_in *addr=NULL;
	char buffer[INET_ADDRSTRLEN]={'\0'},recv[INET_ADDRSTRLEN]={'\0'},snd[INET_ADDRSTRLEN]={"127.0.0.1"},**ptr_host,localhst[INET_ADDRSTRLEN]={"localhost"};
	int sfd,snconfd,opt=1,ret=0,noofbytes,i=0,efd,nfds;
	struct epoll_event *ev,ev_arr[MAXFD];
	int flag,sz=sizeof(ev_arr)+sizeof(ev_arr[0]);
	uint16_t port=2001;
	struct hostent *host;
	socklen_t szadrr;
	addr=(struct sockaddr_in*)calloc(0,sizeof(*addr));
	ev=(struct epoll_event *)calloc(0,sizeof(*ev));
	host=gethostbyname(localhst);
	if(host==NULL)
	{
		perror("gethostbyname fail\n");
		printf("%d",errno);
		exit(0);
	}
	for(i=0;(*host).h_addr_list[i]!=NULL;i++)
	{
		printf("---%s",host->h_addr_list[i]);
		if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		{
			perror("socket fail\n");
			close(sfd);
			exit(0);
		}
	}

	ret=setsockopt(sfd,SOL_SOCKET,SO_KEEPALIVE|SO_REUSEADDR,&opt,sizeof(opt));
	if(ret<0)
	{
		perror("set sock opt fail\n");
		printf("%d",errno);
		close(sfd);
		exit(0);
	}

	addr->sin_family=AF_INET;
	addr->sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	addr->sin_port=htons(port);
	/*host=gethostbyaddr(&addr->sin_addr.s_addr,sizeof(*addr),AF_INET);*/
	ptr_host=host->h_addr_list;
	while(*ptr_host!=NULL)
	{
		printf("%s",inet_ntoa(*(struct in_addr*)(*ptr_host)));
		ptr_host++;
	}
	ret=bind(sfd,(struct sockaddr*)addr,sizeof(*addr));
	if(ret<0)
	{
		perror("bind fail\n");
		printf("%d",errno);
		close(sfd);
		exit(0);
	}

	ret=listen(sfd,5);
	if(ret<0)
	{
		perror("listen fail\n");
		printf("%d",errno);
		close(sfd);
		exit(0);
	}
	printf("no of fd to add in kernael and thier sz+1=%d\n",sz);
	efd=epoll_create1(0); //0or 1
	if(efd==-1)
	{
		perror("epoll create fail\n");
		exit(0);
	}
	//ev->events|=EPOLLET;
	ev->events|=EPOLLIN;
	ev->data.fd=sfd;
	ret=epoll_ctl(efd, EPOLL_CTL_ADD, sfd, ev);
	if(ret==-1)
	{
		perror("epoll_ctl: listen_sock");
		exit(0);
	}
	while(i<2)
	{
		nfds=epoll_wait(efd, ev_arr, MAXFD, -1);
		if(ret<0)
		{
			perror("poll fail\n");
			exit(0);
		}

		for(int fd=0;fd<nfds+1;fd++)
		{
				if(ev_arr[fd].data.fd<0)
					continue;
				if((ev_arr[fd].events & (EPOLLIN))==(EPOLLIN))//2nd false
				{
					if(ev_arr[fd].data.fd==sfd)
					{
					szadrr=sizeof(*addr);
					printf("sz addr%d\n",szadrr);
					snconfd=accept(sfd,(struct sockaddr *)addr,&szadrr);
					if(snconfd<0)
					{
						perror("acept fail\n");
						printf("%d",errno);
						close(snconfd);
						exit(0);
					}
				}
				flag=fcntl(ev_arr[fd].data.fd,F_GETFL);
				printf("file descriptor add mode %d\n",flag);
				//ev->events|=EPOLLET;
				ev->events|=EPOLLIN;
				ev->data.fd=snconfd;
				if(-1==epoll_ctl(efd, EPOLL_CTL_ADD, snconfd, ev))
				{
					perror("new fd not added in ev\n");
					exit(0);
				}
				printf("sz addr %d fd %d\n",szadrr,ev_arr[fd].data.fd);
				if(addr->sin_family==AF_INET)
				{
					inet_ntop(AF_INET,&(*addr).sin_addr.s_addr,buffer,sizeof(buffer));
					printf("check in accept address %s\n",buffer);
				}

			}
			else
			{

				noofbytes=recvfrom(ev_arr[fd].data.fd,buffer,sizeof(buffer),0,(struct sockaddr *)addr,&szadrr);
				if(noofbytes<0)
				{
					perror("recv from fail\n");
					printf("%d",errno);
					exit(0);
				}else if(noofbytes==0)
				{
					printf("recv socket close by client %d\n",ev_arr[fd].data.fd);
					if(-1==epoll_ctl(efd, EPOLL_CTL_DEL, ev_arr[fd].data.fd, ev))
					{
						perror("epoll DEL fail\n");
						exit(0);
					}else if(ev_arr[fd].data.fd==-1)
					{
						perror("new fd is -1\n");
						close(ev_arr[fd].data.fd);
					}
					exit(0);
				}

				printf("no of bytes %d recv %s \n",noofbytes,buffer);
				memcpy(recv,buffer,sizeof(recv));
				memcpy(snd,buffer,sizeof(snd));
				noofbytes=sendto(ev_arr[fd].data.fd,buffer,sizeof(buffer),0,(struct sockaddr *)addr,sizeof(*addr));
				if(noofbytes<0)
				{
					perror("send to fail\n");
					printf("%d",errno);
					exit(0);
				}else if(noofbytes==0)
				{
					printf("send socket close by client %d\n",ev_arr[fd].data.fd);
					if(-1==epoll_ctl(efd, EPOLL_CTL_DEL, ev_arr[fd].data.fd, ev))
					{
						perror("epoll DEL fail\n");
						exit(0);
					}else if(ev_arr[fd].data.fd==-1)
					{
						perror("new fd is -1\n");
						close(ev_arr[fd].data.fd);
					}
					exit(0);
				}
				printf("no of bytes snd %d %s\n",noofbytes,snd);
				memset(buffer,'\0',sizeof(buffer));
				/*if(1<i)
				{
					close(snconfd);
					close(sfd);
				}*/
			}
		}
		i++;
	}
	free(addr);
	close(efd);
	return 0;
}
#endif
