#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int num_send_sig;
int num_recv_sig = 0;
int num_recv_ack = 0;
pid_t pid;

/* DO NOT MODIFY/ADD CODE ABOVE */

/* Fill 4 functions below */
void recv_ack_handler(int sig){
    /* Code here */
	char *msg1 = "all signals have been sent\n";
	num_recv_ack++;
	if(num_send_sig!=num_recv_ack) return;
	else
	{ 
		kill(pid, SIGINT);
		printf("%s", msg1);
		exit(0);
	}
}

void terminate_handler(int sig){
	char *msg = "receiver: total received signals: %d\n";
	/* Code here */
	printf(msg, num_recv_sig);
	exit(0);
}

void sending_handler(int sig){	
	char *msg2 = "sender: total remaining signals: %d\n";
	/* Code here */

	printf(msg2,num_send_sig-num_recv_ack);
	for(int i=0; i<(num_send_sig-num_recv_ack);i++)
		kill(pid, SIGUSR1);
	alarm(1);
}

void sending_ack(int sig){
	char *msg = "receiver: received #%d signal and sending ack\n";
	/* Code here */
	num_recv_sig++;
	printf(msg, num_recv_sig);
	if(num_recv_sig == num_send_sig) signal(SIGUSR1, SIG_IGN);
	kill(pid, SIGUSR1);
}



int main(int argc, char *argv[]) {
	/* DO NOT MODIFY ANY SINGLE CHARACTER IN MAIN */
	num_send_sig = atoi(argv[1]);
	printf("total number of signals: %d\n", num_send_sig);

	if ((pid = fork()) == 0) {
		/* child */
		pid = getppid(); // get parent's pid
		signal(SIGUSR1, sending_ack);
		signal(SIGINT, terminate_handler);
		while(1); // go to inf loop
	}
	else {
		/* parent */
		signal(SIGUSR1, recv_ack_handler);
		signal(SIGALRM, sending_handler);
		alarm(1); // wake me up after 1 sec
		while(1); // go to inf loop
	}
	return 0;
}
/* DO NOT ADD CODE BELOW */
