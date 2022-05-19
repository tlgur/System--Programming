#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 200

int main(int argc, char *argv[]){
	int n, cfd;
	int fd;
	int SIZE=0;
	int cnt=0;
	struct hostent *h;
	struct sockaddr_in saddr;
	char buf[MAXLINE];
	char *host = argv[1];
	int port = atoi(argv[2]);
	char name[50]={0,};

	if((cfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("socket() failed.\n");
		exit(1);
	}

	if((h = gethostbyname(host)) == NULL){
		printf("Invalid hostname %s\n", host);
		exit(2);
	}

	memset((char*)&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	memcpy((char*)&saddr.sin_addr.s_addr, (char*)h->h_addr, h->h_length);
	saddr.sin_port = htons(port);

	if(connect(cfd,(struct sockaddr *)&saddr, sizeof(saddr)) < 0){
		printf("connect() failed.\n");
		exit(3);
	}

	while(1){
		memset(name, 0, 50);
		SIZE=0;
		cnt=0;
		fgets(name, 50, stdin);
		name[strlen(name)-1]='\0';
		write(cfd, name, strlen(name)+1);
		if(!strcmp(name, "quit")) break;
		fd = open(name, O_RDONLY);
		while((n=read(fd, buf, MAXLINE)) >0) SIZE+=n;
		lseek(fd, 0, SEEK_SET);
		while((n=read(fd, buf, MAXLINE)) >0){
			cnt+=n;
			printf("READING..(%d/%d)\n", cnt, SIZE);
			write(cfd, buf, n);
		}
		printf("SENDING FINISH : TOTAL %d bytes.\n", cnt);
		close(fd);
	}
	
	close(cfd);
	return 0;
}
