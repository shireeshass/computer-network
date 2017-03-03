#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sem.h>
#define shmkey  1234
#define pkey 1257
#define semkey1 124
#define semkey2 156
#define shmsize 10
int *pid;
char *spointer;
int fw,fr;
char rd[10];
int scpid;
void *ptread(void *num){
	while(1){
		char buf[1024];
		read(fr,buf,1024);
		fprintf(stdout, "%s\n",buf );
	}
}
void s_wait (int semid){
	printf("waiting\n");
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
void *ptwrite(void *num){
	while(1){
		char buf[1024];
		fscanf(stdin,"%s",buf);
		write(fw,buf,1024);
	}
}
main(int argc,char *argv[]){
	if(argc<2)
	{
		printf("arguements missing\n");
		exit(0);
	}
	int sem1 = sem_init(semkey1,0);
	int sem2 = sem_init(semkey2,0);

	char ws[5] ; ws[0]='r';ws[1]='s';ws[2]=argv[1][0];ws[3]='\0';
	char rs[5] ; rs[0]='w';rs[1]='s';rs[2]=argv[1][0];rs[3]='\0';
 	int sc = open("ms",O_WRONLY);
 	printf("%s\n",ws);
 	char msg[32];
 	int x = getpid();
 	sprintf(msg,"%s%s%d%s",argv[1],"|",x,"|");
 	printf("%s\n",msg);
 	write(sc,msg,sizeof(msg));
 	s_wait(sem1);
 	printf("yessssssss\n");
 	fw = open(ws,O_WRONLY);
		if(fw == -1){
			printf("open error!!\n");
			exit(0);
		}
 	fr = open(rs,O_RDONLY);
	if(fr==-1){
		printf("open error!!\n");
		exit(0);
	}
	

 	pthread_t pwrite,pread;
	pthread_create(&pread,NULL,ptread,(void*)0);
	pthread_create(&pwrite,NULL,ptwrite,(void*)0);
	pthread_join(pread,NULL);
	pthread_join(pwrite,NULL);
}