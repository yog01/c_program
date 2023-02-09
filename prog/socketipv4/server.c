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
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>
#include <signal.h>
#include <wait.h>
//#define server 1
#ifndef server
#define e(a) printf("%d\n",a)

void calculate_time(void *t,int *t_time);
void disp_time(int *time);

int main()
{

	pid_t pid;

	int server_fd=-1,soc_opt=-1, flag=0,ret=-1,digit=0,nw_acp=-1,noofbyte=0,port=2111,time[3]={0},s_child;
	struct sockaddr_in *ptr_server_addr,*ptr_client_addr,server_addr,client_addr;
	volatile register int i=0;
	socklen_t opt_sz,server_sz,client_sz, sz_chk_addrlen;
	char chk_lclhst[INET_ADDRSTRLEN]={'\0'},lcalhst[INET_ADDRSTRLEN]={"127.0.0.1"};
	time_t ti_me=10,*crnt_time=&ti_me;

	ptr_server_addr=&server_addr;
	ptr_client_addr=&client_addr;

	memset(ptr_server_addr,0,sizeof(server_addr));
	memset(ptr_client_addr,0,sizeof(client_addr));

	//socket

	if(0>(server_fd=socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket failed\n");
		e(server_fd);
		close(server_fd);
		return EXIT_FAILURE;
	}
	unlink("server_socket");
	//setsockopt reuse addr
	flag=1;
	opt_sz=sizeof(flag);
	soc_opt=setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT	, &flag,opt_sz);
	if(soc_opt<0)
	{
		perror("socket option fail\n");
		e(soc_opt);
		close(server_fd);
		return EXIT_FAILURE;
	}
	//memset(&server_addr, 0, sizeof(server_addr));
	//bind
		ptr_server_addr->sin_family=AF_INET;
		ptr_server_addr->sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	//inet_aton(lcalhst, &server_addr.sin_addr);
	//inet_pton(AF_INET, lcalhst, &server_addr.sin_addr);
		sz_chk_addrlen=sizeof(ptr_server_addr->sin_addr.s_addr);
	//inet_ntop(AF_INET, &server_addr.sin_addr, chk_lclhst,sz_chk_addrlen);
	printf("addres %s size%d\n",inet_ntoa(ptr_server_addr->sin_addr),sz_chk_addrlen);

	ptr_server_addr->sin_port=htons(port);
	server_sz=sizeof(server_addr);
	ret=bind(server_fd,(struct sockaddr*)ptr_server_addr, server_sz);
	if(ret<0)
	{
		perror("bind failed addressing\n");
		e(ret);
		close(server_fd);
		return EXIT_FAILURE;
	}
	//listen

	ret=listen(server_fd, 5);
	if(ret<0)
	{
		perror("listen fail\n");
		e(ret);
		close(server_fd);
		return EXIT_FAILURE;
	}
	//signal(SIGCHID,SIG_IGN);
	while(i<5)
	{
	//accept
		client_sz=sizeof(client_addr);
		nw_acp=accept(server_fd, (struct sockaddr*)ptr_client_addr, &client_sz);
		if(nw_acp<0)
		{
			perror("accept fail\n");
			e(ret);
			close(server_fd);
			return EXIT_FAILURE;
		}
		calculate_time(crnt_time, time);
		i++;
		pid=fork();
		switch(pid)
		{
		//send
		case -1:
			printf("fork fail\n");
			break;

		case 0:
			noofbyte=sendto(nw_acp, time, sizeof(time), flag, (struct sockaddr*)ptr_client_addr, client_sz);
			if(noofbyte==0)
			{
				perror("failed to send\n");
				e(noofbyte);
				close(nw_acp);
				return EXIT_FAILURE;
			}
			printf("no of bytes send\n");
			e(noofbyte);
			disp_time(time);
			sleep(5);
		//recv
			//noofbyte=recv(nw_acp,time, sizeof(time), 0);
			noofbyte=recvfrom(nw_acp, time, sizeof(time), 0, NULL,NULL);
			if(0>noofbyte)
			{
				perror("recveded msg failed\n");
				e(noofbyte);
				close(nw_acp);
				return EXIT_FAILURE;
			}
			printf("no of bytes rceived\n");
			e(noofbyte);
			disp_time(time);
			break;
		default:
			ret=waitpid(pid, &s_child, WNOHANG);
			if(0>ret)
			{
				perror("child not dead\n");
				e(noofbyte);
				return EXIT_FAILURE;
			}
			printf("if not dead child process id %d",s_child);
			if(i>5)
			{
				shutdown(nw_acp, SHUT_RDWR);
				ret=close(nw_acp);
				if(0>ret)
				{
					perror("close of filedesp fail\n");
					return EXIT_FAILURE;
				}
			}
			break;
		}
	}

	return 0;
}

void calculate_time(void *t,int *t_time)
{

	time_t *crnt_time=(time_t*)t;
	time_t temp=*crnt_time,rem_min=0,i=0,sec=0;
	time(&temp);
	printf(" %ld\n",temp);

	t_time[0]=temp/3600;
	rem_min=temp-(t_time[0]*3600);
	while(rem_min>0)
	{
		++t_time[1];
		rem_min=rem_min/60;
	}
	while(i<3)
	{
		sec=(t_time[0]+t_time[1])*60;
		++i;
	}
	t_time[2]=temp-sec;
}

void disp_time(int *time)
{
	int i=0;
	for(i=0;i<3;++i)
	{
		printf("%d ",time[i]);
	}
	printf("\n");
}

#endif
