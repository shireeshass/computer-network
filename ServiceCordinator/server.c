#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#define shmkey  1234
#define pkey 1257
#define semkey1 124
#define semkey2 156
#define shmsize 10
int *pid;
char *spointer;
int fw,fr,sem1,sem2;
void s_wait (int semid){
	struct sembuf se = {0,-1,0};
	semop(semid,&se,1);
}
void s_signal (int semid){
	printf("signaling\n");
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
char rd[10];
int scpid=0;
void *fiforead(void *num){
	while(1){
		char buf[1024];
		read(fr,buf,1024);
		fprintf(stdout, "%s\n",buf );
	}
}
void *fifowrite(void *num){
	while(1){
		char buf[1024];
		fscanf(stdin,"%s",buf);
		write(fw,buf,1024);
	}
}
void handler1(){
	printf("handler1 is on.............\n");
	printf("%s\n",spointer);
	char *sp = spointer;
	int i=0;
	while(*sp!='|')
	{

		rd[i] = *sp;
		printf("%c  ",rd[i] );
		sp++;
		i++;
	}

	
	memset(spointer,'\0',10);
	
	char ws[5] ; ws[0]='w';ws[1]='s';ws[2]=rd[0];ws[3]='\0';
	char rs[5] ; rs[0]='r';rs[1]='s';rs[2]=rd[0];rs[3]='\0';
	if(mkfifo(ws,0666) == -1){
		printf("mkfifo error ws\n");
		exit(0);
	}
	 if(mkfifo(rs,0666) == -1){
	 	printf("mkfifo error\n");
	 	exit(0);
	 }
	
s_signal(sem1);
	fr = open(rs,O_RDONLY);
	if(fr==-1){
		printf("open error!!\n");
		exit(0);
	}
	fw = open(ws,O_WRONLY);
		if(fw == -1){
			printf("open error!!\n");
			exit(0);
		}
		printf("Awesomee\n");

	pthread_t pwrite,pread;
	pthread_create(&pread,NULL,fiforead,(void*)0);
	pthread_create(&pwrite,NULL,fiforead,(void*)0);
	pthread_join(pread,NULL);
	pthread_join(pwrite,NULL);
}
main(int argc,char *argv[]){
	int shmid = init(shmkey);
	signal(SIGUSR2,handler1);
	int shpkey = init(pkey);
	 sem1 = sem_init(semkey1,0);
	 sem2 = sem_init(semkey2,0);
	pid = (int*)shmat(shmid,NULL,0);
	if(pid == (int*)-1)
	{
		printf("shmat error!!\n");
		exit(0);
	}
	spointer = (char*)shmat(shpkey,NULL,0);
	if(spointer == (char*)-1){
		printf("Shmget error2!!\n");
	}
	scpid= *pid;
	printf("%d\n",scpid);
	*pid = getpid();
	printf("process id is %d\n",*pid);
	s_signal(sem2);
	sleep(5);
	printf("signal sent\n");
	
	while(1){signal(SIGUSR2,handler1);}
	
}