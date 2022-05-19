#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int cfd, n;
	int cnt =0;
	struct sockaddr_in addr;
	struct hostent * h;
	char *host = argv[1];
	int port = atoi(argv[2]);
	char buf[20] = {0,};
	char answer[1<<10] = {0,};

	if((cfd = socket(AF_INET, SOCK_STREAM, 0)) <0){
		perror("socket failed\n");
		exit(1);
	}
	
	if((h = gethostbyname(host)) == NULL ) {
		perror("invalid hostname");
		exit(2);
	}

	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	memcpy((char*)&addr.sin_addr.s_addr, (char *)h->h_addr, h->h_length);
	addr.sin_port = htons(port);

	if(connect(cfd, (struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		perror("connect() failed\n");
		exit(3);
	}

	read(cfd, buf, 20);
	printf("%s", buf);
	if(!strcmp(buf, "BUSY SERVER\n")){
		close(cfd);
		return 0;
	}
	while(1){
		cnt=0, n=0;
		memset(buf, 0, 20);
		memset(answer, 0, 1<<10);
		fgets(buf, 20, stdin);
		buf[strlen(buf)-1] = '\0';

		if(!((buf[0] == '[') && (buf[strlen(buf)-1] == ']'))){
			printf("WRONG COMMAND\n");
			continue;
		}

		write(cfd, buf, 20);
		while(buf[cnt++] != ' ');
		n= buf[cnt] -'0';
		if(n==0){
			read(cfd, answer, 1<<10);
			break;
		}
		else if(n==3) read(cfd, answer, 1<<10);
		else read(cfd, answer, 3);

		printf("%s\n", answer);
	}	


	close(cfd);
	printf("connection closed\n");
	printf("%s\n", answer);
	return 0;
}
