#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>
#define portno 50001
int shmkey = 1234;
int shmsize = 10;
int *ptr;
struct serv_cnt{
	int port_no;
	int clcnt;
	char sv[6];
	int maxcnt;
};
struct serv_cnt serv[5];
int  init(int key){
	int shmid = shmget(key,shmsize,IPC_CREAT|0666);
	if(shmid <=0){
		printf("shmget error!!\n");
		exit(0);
	}
	return shmid;
}
void signalhandler(){
	int cptr = *ptr;
	*ptr = getpid();
	int i=0;
	for(i=0;i<5;i++)
	{
		if(serv[i].port_no == cptr)
			break;
	}
	serv[i].clcnt --;
}
int in=0;
int rec[20][2];
void handlechld(){
	
	int i=0,status;
	for(i=0;i<in;i++)
	{
		pid_t result = waitpid(rec[i][0], &status, WNOHANG);	
		if(result >0)
		{
			serv[rec[i][1]].clcnt --;
			break;
		}
	}
}
int main(){
	int shmid = init(shmkey);
	
	signal(SIGUSR1,signalhandler);
	struct sockaddr_in serv_addr,cli_addr;
	int sfd,i;
	ptr = (int*)shmat(shmid,NULL,0);
	*ptr = getpid();
	
	for(i=0;i<5;i++)
	{
		serv[i].port_no = 50005+i;
		serv[i].clcnt=0;
		sprintf(serv[i].sv,"%s%d","./serv",i+1);
		serv[i].maxcnt = 2;
	}
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0){
		printf("sfd error!!\n");
		exit(0);
	}
	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//printf("%d\n",portno.porno);
	serv_addr.sin_port = htons(portno);//htons  converts a port number in host byte order to a port number in network byte order
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
	{
		printf("bind Error!!\n");
		exit(0);
	}
	
	listen(sfd,5);
	while(1){
		signal(SIGCHLD,handlechld);
		int cllength = sizeof(cli_addr);
		int nsfd = accept(sfd,(struct sockaddr*)&cli_addr,&cllength);
		printf("accepted\n");
		if(nsfd < 0){
			printf("accept error!!\n");
			exit(0);
		}
		char buf[256];
		recv(nsfd,buf,256,0);
		int pno=atoi(buf);
		printf("%d\n",pno );
		for(i=0;i<5;i++)
		{
			if(serv[i].port_no == pno)
				break;
		}
		rec[in++][0] =serv[i].port_no;
		rec[in++][0]  = i;
		if(serv[i].clcnt<serv[i].maxcnt)
		{
			bzero(buf,256);
			serv[i].clcnt++;
			printf("\nhere it is %d\n",serv[i].clcnt);
			sprintf(buf,"%s","accept");
			send(nsfd,buf,strlen(buf),0);
			if(serv[i].clcnt == 1){
				printf("forked\n");
				int pid;
			if((pid = fork())==0)
			{
				printf("execing  %s\n\n",serv[i].sv);
				execlp(serv[i].sv,serv[i].sv,NULL);
				printf("EXEC error!!\n");
				serv[i].clcnt--;
			}

		}
		}
		else{
			bzero(buf,256);
			sprintf(buf,"%s","reject");
			send(nsfd,buf,strlen(buf),0);
		}
	}
}