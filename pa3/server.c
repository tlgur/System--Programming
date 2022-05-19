#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

int passcode[1<<10];
int seat[1<<8];
pthread_mutex_t m_seat[1<<8];
int client_connected = 0;
pthread_t user_login[1<<10];
pthread_mutex_t connection= PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	int user;
	int action;
	int data;
}query;

void* handling_client(void* arg){
	int cfd = *((int *)arg);
	int cnt;
	char flag_login = 0;
	char ack[3] = {0,};
	query Q;
	char buf[20] = {0,};
	char r_seat[1<<10] = {0,};
	pthread_detach(pthread_self());
	free(arg);

	write(cfd, "server connected\n", 20);
	while(1){
		cnt =1;
		memset(r_seat, 0, 1024);
		memset(&Q, 0, sizeof(query));
		read(cfd, buf, 20);
		if(buf[cnt]=='-'){
			printf("Negative user\n");
			write(cfd, "-1", 3);
			continue;
		}
		while(buf[cnt] != ','){
			Q.user = 10*Q.user + (buf[cnt] - '0');
			cnt++;
		}
		if(Q.user>1023){
			printf("user id : out of range\n");
			write(cfd, "-1", 3);
			continue;
		}
		cnt += 2;
		if(!((buf[cnt] <= 53) && (buf[cnt] >=48))){
			printf("ACTION : Out of range\n");
			write(cfd, "-1", 3);
			continue;
		}
		Q.action = buf[cnt] - '0';
		cnt += 3;
		if(buf[cnt]=='-'){
			printf("Negative data\n");
			write(cfd, "-1", 3);
			continue;
		}
		while(buf[cnt] !=']'){
			Q.data = 10*Q.data + (buf[cnt] -'0');
			cnt++;
		}

		printf("[%d %d %d]\n", Q.user, Q.action, Q.data);

		if(Q.action==0){
			pthread_mutex_lock(&m_seat[0]);
			sprintf(r_seat, "%d", seat[0]);
			pthread_mutex_unlock(&m_seat[0]);
			for(int i = 1; i<256 ; i++)
			{
				pthread_mutex_lock(&m_seat[i]);
				sprintf(r_seat, "%s %d", r_seat, seat[i]);
				pthread_mutex_unlock(&m_seat[i]);
			}
			write(cfd, r_seat, 1<<10);
			printf("terminate the connect\n");
			break;
		}
		switch(Q.action){
			case 1:
				if((flag_login == 1) || (user_login[Q.user] != 0 )){
					printf("action 1, fault - already logged in\n");
					write(cfd, "-1", 3);
					break;
				}

				if((passcode[Q.user] != -1) && (passcode[Q.user] != Q.data)){
					printf("action 1, fault - wrong passcode\n");
					write(cfd, "-1", 3);
					break;
				}
				if(passcode[Q.user] == -1) passcode[Q.user] = Q.data;
				flag_login = 1;
				user_login[Q.user] = pthread_self();
				write(cfd, "1", 3);
				printf("log in : %d\n", Q.user);
				break;
			case 2:
				if(!pthread_equal(pthread_self(), user_login[Q.user])){
					printf("action %d, fault - different user\n", Q.action);
					write(cfd, "-1", 3);
					break;
				}
				if((Q.data > 255) || (seat[Q.data] != -1)){
					printf("action 2, fault - unavailable seat\n");
					write(cfd, "-1", 3);
					break;
				}
				pthread_mutex_lock(&m_seat[Q.data]);
				seat[Q.data] = Q.user;
				sprintf(ack, "%d", Q.data);
				write(cfd, ack, 3);
				printf("Reserved seat: %d, user: %d\n", Q.data, Q.user);
				pthread_mutex_unlock(&m_seat[Q.data]);
				break;
			case 3:
				if(!pthread_equal(pthread_self(), user_login[Q.user])){
					printf("action %d, fault- different user\n", Q.action);
					write(cfd, "-1", 3);
					break;
				}
				for(int i=0 ; i<256; i++){
					pthread_mutex_lock(&m_seat[i]);
					if(seat[i]==Q.user){
						if(r_seat[0] == '\0') sprintf(r_seat, "%d", i);
						else sprintf(r_seat, "%s %d", r_seat, i);
					}
					pthread_mutex_unlock(&m_seat[i]);
				}
				if(r_seat[0] == '\0') {
					printf("action 3 fault - user with no seat\n");
					write(cfd, "-1", 3);
				}
				else {
					printf("printing all reserved seat\n");
					write(cfd, r_seat, 1<<10);
				}
				break;
			case 4:
				if(!pthread_equal(pthread_self(), user_login[Q.user])){
					printf("action %d, fault- different user\n", Q.action);
					write(cfd, "-1", 3);
					break;
				}
				pthread_mutex_lock(&m_seat[Q.data]);
				if(seat[Q.data] != Q.user) {
					printf("action 4 fault - Not your seat\n");
					write(cfd, "-1", 3);
				}
				else
				{
					sprintf(ack, "%d", Q.data);
					seat[Q.data] = -1;
					write(cfd, ack, 3);
					printf("Canel the seat : %d\n", Q.data);
				}
				pthread_mutex_unlock(&m_seat[Q.data]);
				break;
			case 5:
				if(!pthread_equal(pthread_self(), user_login[Q.user])){
					printf("action %d, fault- different user\n", Q.action);
					write(cfd, "-1", 3);
					break;
				}
				flag_login = 0;
				memset(&user_login[Q.user], 0, sizeof(pthread_t));
				write(cfd, "1", 3);
				printf("logged out : %d\n", Q.user);
				break;
		}
	}
	close(cfd);
	pthread_mutex_lock(&connection);
	client_connected--;
	pthread_mutex_unlock(&connection);
	pthread_exit(NULL);
}

void handler(int sig){
	for(int i=0; i<256; i++)
		pthread_mutex_destroy(&m_seat[i]);
	pthread_mutex_destroy(&connection);
	exit(0);
}

int main(int argc, char * argv[]){
	int lfd;
	int caddrlen;
	int * cfd;
	struct hostent * h;
	struct sockaddr_in addr, caddr;
	int port = atoi(argv[1]);
	pthread_t tid;

	signal(SIGINT, handler);

	for(int i=0; i<256; i++)
		pthread_mutex_init(&m_seat[i], NULL);

	memset(seat, -1, sizeof(int)*(1<<8));
	memset(user_login, 0, sizeof(pthread_t)*(1<<10));
	memset(passcode, -1, sizeof(int)* (1<<10));

	if((lfd = socket(AF_INET, SOCK_STREAM, 0)) <0 ){
		perror("socket failed\n");
		exit(1);
	}

	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if(bind(lfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind failed\n");
		exit(2);
	}

	if(listen(lfd, 1<<7) < 0 ){
		perror("listen failed\n");
		exit(3);
	}

	while(1){
		cfd = (int *)malloc(sizeof(int));
		if((*cfd = accept(lfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) <0 ){
			perror("accept failed\n");
			free(cfd);
			continue;
		}
		if(client_connected > 1024){
			write(*cfd, "BUSY SERVER\n", 13);
			close(*cfd);
			free(cfd);
			continue;
		}
		pthread_mutex_lock(&connection);
		client_connected++;
		pthread_mutex_unlock(&connection);
		pthread_create(&tid, NULL, handling_client, (void*) cfd);
	}

}

