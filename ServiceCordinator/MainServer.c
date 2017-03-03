#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <sys/sem.h>
#include <fcntl.h>
#define shmkey  1234
#define pkey 1257
#define semkey1 124
#define semkey2 156
#define shmsize 10
int *pid,sem1,sem2;
void s_wait (int semid){
	struct sembuf se = {0,-1,0};
	semop(semid,&se,1);
}
void s_signal (int semid){
	struct sembuf se = {0,1,0};
	semop(semid,&se,1);
}
int sem_init(int key,int initial_value)
{
	int semid = semget(key,1,0666|IPC_CREAT);
	if(semid == -1)
	{
		printf("semget error!!\n");
		exit(0);
	}
	semctl(semid,0,SETVAL,initial_value);
	return semid;
}
int  init(int key){
	int shmid = shmget(key,shmsize,IPC_CREAT|0666);
	if(shmid <=0){
		printf("shmget error!!\n");
		exit(0);
	}
	return shmid;
}
int proid[10];int n=0;

void handler2(){
	printf("server is on....\n");
}
main(int argc,char *argv[]){
	int shmid = init(shmkey);
	int shpid = init(pkey);
	sem1 = sem_init(semkey1,0);
	 sem2 = sem_init(semkey2,0);
	mkfifo("ms",0666);
	pid = (int*)shmat(shmid,NULL,0);
	
	printf("process id is %d\n",*pid);
	char *spointer = (char*)shmat(shpid,NULL,0);
	int fd = open("ms",O_RDONLY);
	char buf[32];
	
	while(1){

		signal(SIGUSR2,handler2);
		*pid = getpid();
		if(read(fd,buf,20) == -1)
		{
			printf("read error!!\n");exit(0);
		}
		printf("%s\n",buf);
		s_wait(sem2);
		proid[n]=*pid;
		printf("%d\n",*pid);
		n++;
		memcpy(spointer,buf,sizeof(buf));
		printf("%s\n",spointer);
		printf("%d\n",*spointer - '0');	
		sleep(5);
		kill(*pid,SIGUSR2);
	}
}