/*
 * pipe.c
 *
 *  Created on: Oct 27, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#define e(p) printf("%s\n",p)

int main()
{

	int fl_pipe[2],st,stat_chlid,data_processed;
	pid_t pid,pid_child;
	char buffer[BUFSIZ];
	char writ[BUFSIZ]="abcd\n";
	st=pipe(fl_pipe);
	if(st==-1)
	{
		e("pipe is not created");
	}
	pid=fork();

	switch(pid)
	{
		case -1:
			e("child process failed");
			break;
		case 0:
			close(fl_pipe[0]);
			e("write data in pipe");
			data_processed=write(fl_pipe[1],writ,BUFSIZ);
			if(data_processed<0)
			{
				e("failed to write at pipe");
			}
			printf("no of bytes written %d\n",data_processed);
			//sleep(30);
			break;
		default:
			printf("parent id %d child %d\n",getppid(),getpid());
			close(fl_pipe[1]);
			e("read from pipe");
			pid_child=waitpid(pid, &stat_chlid, WNOHANG);
			if(pid_child<0)
			{
				e("chlid hang state");
				printf("%d",stat_chlid);
			}
			data_processed=read(fl_pipe[0],buffer,BUFSIZ);
			if(data_processed<0)
			{
				e("failed to read from pipe");
			}
			printf("no of bytes readed %d\n",data_processed);
			e(buffer);
			close(fl_pipe[0]);
		break;
	}
	return 0;
}
