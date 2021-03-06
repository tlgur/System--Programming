#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 80

int main(int argc, char* argv[]){
	int n, listenfd, connfd, caddrlen;
	int fd;
	struct hostent *h;
	struct sockaddr_in saddr, caddr;
	char buf[MAXLINE];
	int port = atoi(argv[1]);
	char name[50] = {0,};

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("socket() failed\n");
		exit(1);
	}
	
	memset((char*)&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if(bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0){
		printf("bind() failed\n");
		exit(2);
	}

	if(listen(listenfd, 5) < 0 ){
		printf("listen() failed\n");
		exit(3);
	}

	while(1){
		caddrlen = sizeof(caddr);
		if((connfd = accept(listenfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) < 0){
			printf("accept failed\n");
			continue;
		}
		n = read(connfd, name, 50);
		printf("START COPY : %s\n", name);
		strcat(name, "_copy");
		fd = open(name, O_WRONLY|O_CREAT, 0644);
		while((n = read(connfd, buf, MAXLINE)) > 0)
			write(fd, buf, n);
		printf("COPY FINISH\n");
		close(fd);
		printf("connection terminated\n");
		close(connfd);
	}
}
