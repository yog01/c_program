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
#include <time.h>

#define e(a) printf("%d\n",a);
#define SIZE 50

#define client 0
#ifndef client

int get_host_time(char *host_addr,struct sockaddr_in *ptr_client_addr,int file_desp,int *flag,int *port);
void calculate_time(void *t,int *t_time);
void disp_time(int *time);
int main()
{
	int file_desp=-1,ret=0,flag=0,digit=1,noofbytes,por_t=2111,svc_al_sz=0,opt_name=1,i=0,time[3]={0};
	char port[SIZE]={"2111"},client_addr_chk[INET_ADDRSTRLEN]={'\0'},client_addr_buf[INET_ADDRSTRLEN]={"185.125.190.56"},serv_name[SIZE]={"time"},**ptr_svc=NULL,**ptr_addrlist=NULL,buffer[SIZE]={"127.0.0.1"};
	struct sockaddr_in client_addr,*ptr_client_addr;
	socklen_t sz_client,sz_server,sz_opt_nm;
	struct sockaddr server_addr;
	struct addrinfo *hints,*addrs,a_hints,*p;
	struct servent *ptr_svc_info,srvc_info;
	struct hostent *ptr_host,host_info;
	hints=&a_hints;//Initalize
	ptr_svc_info=&srvc_info;
	ptr_host=&host_info;
	ptr_client_addr=&client_addr;
	time_t time_1=10,*ptr_time=&time_1;
	//information

	memset(hints,0,sizeof(struct addrinfo));
	memset(ptr_svc_info, 0, sizeof(struct servent));
	memset(ptr_host, 0, sizeof(struct hostent));
	memset(ptr_client_addr, 0, sizeof(struct sockaddr_in));

	hints->ai_family=AF_INET;
	hints->ai_flags=AI_PASSIVE;
	hints->ai_socktype=SOCK_STREAM;
	hints->ai_protocol=IPPROTO_TCP;
	ret=getaddrinfo(NULL,port,hints,&addrs);
	if(0>ret)
	{
		perror("get addr info fail");
		e(ret);
		return EXIT_FAILURE;
	}

	ptr_svc_info=getservbyname(serv_name, "udp"); //same as protocol and port service name
	if(!ptr_svc_info)
	{
		printf("not able to get services\n");
		return EXIT_FAILURE;
	}
	printf("local host srv name%s\n",ptr_svc_info->s_name);
	printf("service port%d\n",ntohs(ptr_svc_info->s_port));
	ptr_svc=ptr_svc_info->s_aliases;
	svc_al_sz=strlen((char *)*ptr_svc);
	while(*ptr_svc)
	{
		printf("ptr to srvc%s\n",*ptr_svc);
		ptr_svc++;
	}


	for(p=addrs;p!=NULL;p=addrs->ai_next)
	{
		//socket
		file_desp=socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
		if(0>file_desp)
		{
			perror("socket fail\n");
			e(file_desp);
			close(file_desp);
			return EXIT_FAILURE;
		}
	//create socketby hostent
	#ifdef client
		gethostname(buffer, sizeof(buffer));
		ptr_host=gethostbyname2(buffer, AF_INET);
		while(NULL!=ptr_host->h_addr_list[i])
		{
			file_desp=socket(AF_INET, SOCK_STREAM, 0);
			if(0>file_desp)
			{
				perror("socket creation fail\n");
				e(file_desp);
				close(file_desp);
				return EXIT_FAILURE;
			}
			i++;
		}
	#endif
		sz_opt_nm=sizeof(opt_name);
		ret=setsockopt(file_desp, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt_name, sz_opt_nm);
		if(0>ret)
		{
			perror("set sockoption fail\n");
			e(ret);
			close(file_desp);
			return EXIT_FAILURE;
		}
	}
	//if check services or connect and file_desp to updated file_desp
	//nwf_des=get_host_time(client_addr_buf, ptr_client_addr, file_desp,&flag,&por_t);

	//connect
	ptr_client_addr->sin_family=AF_INET;
	ptr_client_addr->sin_addr.s_addr=htonl(INADDR_LOOPBACK); //local host ip 127.0.0.1 //
	//inet_pton(AF_INET,client_addr_buf,&client_addr.sin_addr.s_addr);
	ptr_client_addr->sin_port=htons(por_t);
	sz_client=sizeof(client_addr);
	inet_ntop(AF_INET,&client_addr.sin_addr.s_addr, client_addr_chk, sizeof(client_addr_chk));
	printf("client adder %s addr sz%ld\n",client_addr_chk,sizeof(ptr_client_addr->sin_addr.s_addr));


	ptr_host=gethostbyaddr(&client_addr.sin_addr.s_addr, sizeof(ptr_client_addr->sin_addr),AF_INET);
	if(!ptr_host)
	{
		perror("not to get host address\n");
		return EXIT_FAILURE;
	}

	ptr_addrlist=ptr_host->h_addr_list;
	while(0!=(*ptr_addrlist))
	{
		printf(" %s",inet_ntoa(*(struct in_addr*)*ptr_addrlist)); //address typecast char to struct addr* copy to
		ptr_addrlist++;
	}


	ret=connect(file_desp, (struct sockaddr *)ptr_client_addr,sz_client);
	if(ret<0)
	{
			perror("connect fail\n");
			e(ret);
			close(file_desp);
			return EXIT_FAILURE;
	}
	calculate_time(ptr_time, time);
	//send
	sz_server=sizeof(server_addr);
	noofbytes=sendto(file_desp, time,sizeof(time),flag,&server_addr,sz_server);
	if(noofbytes<0)
	{
		perror("send fail\n");
		e(noofbytes);
		close(file_desp);
		return EXIT_FAILURE;
	}
	printf("no of bytes send\n");
	e(noofbytes);
	disp_time(time);
	//recv
	noofbytes=recv(file_desp, time, sizeof(time), flag);
	if(noofbytes<0)
	{
		perror("recv fail\n");
		e(noofbytes);
		close(file_desp);
		return EXIT_FAILURE;
	}
	printf("no of bytes recved \n");
	e(noofbytes);
	printf("recived digit\n");
	disp_time(time);
	shutdown(file_desp, SHUT_RDWR);
	ret=close(file_desp);
	if(0>ret)
	{
		perror("close file despfail\n");
		return EXIT_FAILURE;
	}

	return 0;
}
#ifndef client
int get_host_time(char *host_addr,struct sockaddr_in *ptr_client_addr,int file_desp,int *flag,int *port)
{
	int noofbyte=0,ret=0;
	struct hostent *host;
	socklen_t len=0,res=0;
	char **ptr,buffer[SIZE]="\0";
	char chk_service_addr[INET_ADDRSTRLEN]="\0",ipaddr_buffer[INET_ADDRSTRLEN]="\0";
	struct sockaddr_in t_address;
	host=gethostbyname(host_addr);

	if(!host)
	{
		perror("host is null\n");
		close(file_desp);
		return EXIT_FAILURE;
	}
	ret =  strcmp(host_addr, "localhost");
	if(ret!=0)
	{
		ptr_client_addr->sin_family=AF_INET;
		ptr_client_addr->sin_port=htons(*port);
		ptr_client_addr->sin_addr.s_addr=htonl(INADDR_LOOPBACK);
		len=sizeof(ptr_client_addr->sin_addr.s_addr);
		t_address.sin_addr.s_addr=ptr_client_addr->sin_addr.s_addr;
		//store
		//void *v_ptr=(struct sockaddr_in*)ptr_client_addr;
		//memcpy(&t_address.sin_addr.s_addr,ptr_client_addr->sin_addr.s_addr,sizeof(uint32_t));

		host=gethostbyaddr(&t_address.sin_addr.s_addr, len, AF_INET);//cannot derefrance void pointer when passing to function
		if(!host)
		{
			perror("host is null gethostbyaddr\n");
			close(file_desp);
			return EXIT_FAILURE;
		}
		ptr=host->h_addr_list;
		while(0!=(*ptr))
		{
			printf("%s",inet_ntoa(*(struct in_addr *)*ptr));
			ptr++;
		}
		printf("sizeof address%d \n",len);
	}
		else
	{
		ptr=host->h_addr_list;
		ptr_client_addr->sin_family=host->h_addrtype;
		ptr_client_addr->sin_port=ntohs(*port);
		ptr_client_addr->sin_addr=*(struct in_addr *)*ptr;
		len=host->h_length;
		printf("sz of time client addr%d\n",host->h_length);
		while(0!=(*ptr))
		{
			printf("%s",inet_ntoa(*(struct in_addr*)*ptr));
			ptr++;
		}

	}



	res=connect(file_desp, (struct sockaddr *)ptr_client_addr,len);
	if(0>res)
	{
		perror("connect faile\n");
		close(file_desp);
		return EXIT_FAILURE;
	}

	*flag=0; //for recv function


	//noofbyte=re(file_desp, buffer, sizeof(buffer));
	noofbyte=recv(file_desp, buffer, sizeof(buffer), *flag);
	if(0>noofbyte)
	{
		perror("recv failed\n");
		e(noofbyte);
		close(file_desp);
		shutdown(file_desp, SHUT_RDWR);
		return EXIT_FAILURE;
	}
	printf("no of bytes %d time %s\n",noofbyte,buffer);
	close(file_desp);
	shutdown(file_desp, SHUT_RDWR);
	return file_desp;
}
#endif
void disp_time(int *time)
{
	int i=0;
	for(i=0;i<3;++i)
	{
		printf("%d ",time[i]);
	}
	printf("\n");
}
void calculate_time(void *t,int *t_time)
{
	time_t *crnt_time=(time_t *)t,temp=0;
	int min=0,hr=0,rem_min=0,sec=0;

	time(crnt_time);
	temp=*crnt_time;
	t_time[0]=temp/3600;
	rem_min=temp-(t_time[0]*3600);
	while(rem_min>0)
	{
		t_time[1]++;
		rem_min=rem_min/60;
	}
	sec=temp-(t_time[0]*3600+rem_min);
	t_time[2]=temp-sec;

}
#endif
