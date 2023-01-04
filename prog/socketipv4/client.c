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
#define SIZE 50
//#define client 1
#ifndef client

int get_host_time(char *host_addr,struct sockaddr_in *ptr_client_addr,int file_desp,int *flag,int *port);

int main()
{
	int file_desp=-1,ret=0,flag=0,nwf_des=0,digit=1,noofbytes,por_t=2111,svc_al_sz=0,opt_name=1;
	char port[SIZE]={"2111"},client_addr_chk[INET_ADDRSTRLEN]={'\0'},client_addr_buf[INET_ADDRSTRLEN]={"localhost"},serv_name[SIZE]={"time"},**ptr_svc=NULL,**ptr_addrlist=NULL;
	struct sockaddr_in client_addr,*ptr_client_addr;
	socklen_t sz_client,sz_server,sz_opt_nm;
	struct sockaddr server_addr;
	struct addrinfo *hints,*addrs,a_hints;
	struct servent *ptr_svc_info,srvc_info;
	struct hostent *ptr_host,host_info;
	hints=&a_hints;//Initalize
	ptr_svc_info=&srvc_info;
	ptr_host=&host_info;
	ptr_client_addr=&client_addr;
	//information

	memset(hints,0,sizeof(struct addrinfo));
	memset(ptr_svc_info, 0, sizeof(struct servent));
	memset(ptr_host, 0, sizeof(struct hostent));
	memset(ptr_client_addr, 0, sizeof(struct sockaddr_in));

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

	ptr_svc_info=getservbyname(serv_name, "tcp"); //same as protocol and port service name
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



	//socket

	if(0>(file_desp=socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol)))
	{
		perror("socket fail\n");
		e(file_desp);
		close(file_desp);
		return EXIT_FAILURE;
	}
	sz_opt_nm=sizeof(opt_name);
	ret=setsockopt(file_desp, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt_name, sz_opt_nm);
	if(0>ret)
	{
		perror("set sockoption fail\n");
		e(ret);
		close(file_desp);
		return EXIT_FAILURE;
	}
	//if check services or connect
	nwf_des=get_host_time(client_addr_buf, ptr_client_addr, file_desp,&flag,&por_t);
	/*//connect
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


	nwf_des=connect(file_desp, (struct sockaddr *)ptr_client_addr,sz_client);
	if(nwf_des<0)
	{
			perror("connect fail\n");
			e(nwf_des);
			close(file_desp);
			return EXIT_FAILURE;
	}
*/
	//send
	/*sz_server=sizeof(server_addr);
	noofbytes=sendto(nwf_des, &digit,sizeof(digit),flag,&server_addr,sz_server);
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
	*/

	close(nwf_des);
	return 0;
}

int get_host_time(char *host_addr,struct sockaddr_in *ptr_client_addr,int file_desp,int *flag,int *port)
{
	int noofbyte=0,ret=0;
	struct hostent *host;
	socklen_t len=0,res=0;
	char **ptr,buffer[SIZE]="\0";
	char chk_service_addr[INET_ADDRSTRLEN]="\0";
	struct sockaddr_in t_address;
	host=gethostbyname(host_addr);

	if(!host)
	{
		perror("host is null\n");
		close(file_desp);
		return EXIT_FAILURE;
	}
	ret=strcmp(host_addr, "localhost");
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


	noofbyte=read(file_desp, buffer, sizeof(buffer));
	if(0>noofbyte)
	{
		perror("recv failed\n");
		e(noofbyte);
		//close(file_desp);
		shutdown(file_desp, SHUT_RDWR);
		return EXIT_FAILURE;
	}
	printf("no of bytes %d time %s\n",noofbyte,buffer);
	return file_desp;
}


#endif
