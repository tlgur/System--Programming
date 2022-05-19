#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h> 
#include <signal.h>
#include <sys/wait.h>

#define T_MSG 20
#define MAX_LEN 512

struct msg {
    long msgtype;
    int sender;
    char text[MAX_LEN];
};

struct msg_rt {
    long msgtype;
    char timestamp[MAX_LEN];
};

void handler(int sig){
	exit(0);
}

int main() {
    key_t key = ftok(".", 'a');
    int my_id, receiver_id, pid;
    struct msg msg;
    struct msg_rt msg_rt;
    struct msqid_ds msqid;

    printf("My id: "); 
    scanf("%d", &my_id);
    getchar();  
    printf("Receiver's id: ");
    scanf("%d", &receiver_id); 
    getchar();


    int qid = msgget(key, IPC_CREAT | 0x1FF); 

    signal(SIGCHLD, handler);

    if (qid < 0) {
	perror("msgget faild");
    }
    /* Please do not modify up to this point */


    if (fork() == 0) {
	while (1) {
	    //FILL YOUR CODE
	    fgets(msg.text, MAX_LEN, stdin);
	    msg.text[strlen(msg.text)-1]='\0';
	    if(!strcmp(msg.text, "quit"))
		    exit(0);
	    msg.sender = my_id;
	    msg.msgtype=receiver_id;
	    if(msgsnd(qid, (void*)&msg, MAX_LEN, 0)==-1){
		    perror("SEND ERR");
		    exit(0);
	    }
	}		

    }
    else {
	while (1) {
	    //FILL YOUR CODE
	    if(msgrcv(qid, (void*)&msg, MAX_LEN, my_id, IPC_NOWAIT)!=-1){
		    printf("RECEIVER: %s\n", msg.text);
		    msg_rt.msgtype = 256 - receiver_id;
		    msgctl(qid, IPC_STAT, &msqid);
		    sprintf(msg_rt.timestamp, "ACK FROM %d AT %s", my_id, ctime(&msqid.msg_rtime));
		    msgsnd(qid, (void *)&msg_rt, MAX_LEN, 0);
	    }
	    if(msgrcv(qid, (void*)&msg_rt, MAX_LEN, 256 - my_id, IPC_NOWAIT)!=-1){
		    printf("%s", msg_rt.timestamp);
	    }
	}
    }	

    return 0;
}
