#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	char temp[1<<10]={0,};
	char* str[10]={0,};
	char* ptr;
	int i=0;

	while(1)
	{
		memset(temp, 0, 1<<10);
		memset(str, 0, 10);
		ptr=NULL;
		i=0;


		fgets(temp, 1<<10, stdin);
		temp[strlen(temp)-1]='\0';


		if(!(strcmp(temp,"quit"))) exit(0);


		ptr=strtok(temp, " ");
		while(ptr!= NULL){
			str[i++]=ptr;
			ptr=strtok(NULL, " ");
		}

		if(fork()==0)
			execvp(str[0], str);

		wait(NULL);
	}
	exit(0);
}
