#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

void EXE(char temp[], char* arg[])
{
	int str_num = 0;
	char* ptr=NULL;

	ptr=strtok(temp, " ");
	while(ptr!=NULL){	
		arg[str_num++]=ptr;
		ptr=strtok(NULL, " ");
	}
	arg[str_num]= '\0';

	execvp(arg[0], arg);
}

int main (){
	pid_t pid= getpid();
	int str_num=0;
	char temp[1<<10]={0,};
	char* ptr=NULL;
	char*arg[1<<4]={0,};

	int fd[2];
	int num_pipe=0;
	char* pipe_arg[1<<4]={0,};

	while(1){
		str_num = 0;
		num_pipe = 0;

		fgets(temp, 1<<10, stdin);
		temp[strlen(temp)-1]= '\0';

		if(!strcmp(temp, "quit")) 
		{
			printf("End Command written\n");
			exit(0);
		}

		if(strchr(temp, '|')!=NULL){
			ptr = strtok(temp, "|");
			while(ptr != NULL){
				pipe_arg[num_pipe++] = ptr;
				ptr = strtok(NULL, "|");
			}
			pipe_arg[num_pipe--] = '\0';
		
			if(pipe(fd)==-1){
				perror("PIPE OPEN ERR\n");
				continue;
			}

			if((pid=fork())==0){
				dup2(fd[1], 1);
				EXE(pipe_arg[0], arg);
			}
			printf("First pid : %d\n", pid);
			printf("End first pipe %d\n", wait(NULL));
			if((pid=fork())==0){
				dup2(fd[0], 0);
				close(fd[0]);	
				EXE(pipe_arg[1], arg);
			}
			printf("Scnd pid : %d\n", pid);
			close(fd[0]); close(fd[1]);
			printf("End Scnd pipe %d\n", wait(NULL));
		}
		else 
		{
			if(fork()==0) EXE(temp, arg);
			wait(NULL);
		}

	}
}
