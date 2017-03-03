#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#define shmkey 1234
#define shmsize 4
int  init(int key){
	int shmid = shmget(key,shmsize,IPC_CREAT|0666);
	if(shmid <=0){
		printf("shmget error!!\n");
		exit(0);
	}
	return shmid;
}
main(){
	char ch;
	int shmid = init(shmkey);
	int *pid = (int*)shmat(shmid,NULL,0);
	while(1){
		printf("Want to send signal");
		scanf("%c",&ch);
		printf("\n");
		kill(*pid,SIGUSR1);
	}
}