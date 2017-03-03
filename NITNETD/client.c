/*
	Author :- Vinay Kant
	
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>	
#include <netdb.h>
#include <sys/shm.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#define portno 50001
int shmkey = 1234;
int shmsize = 10;
int *ptr;
int port_num;
/*
struct sockaddr_in{
	short sin_family; must be AF_INET
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8]; Not used , must be zero	
};
*/
int sfd;

int  init(int key){
	int shmid = shmget(key,shmsize,IPC_CREAT|0666);
	if(shmid <=0){
		printf("shmget error!!\n");
		exit(0);
	}
	return shmid;
}
int *pr;
void sighandler(){
	int pid = *ptr;
	//int p2 = *pr;
	*ptr = port_num;
	kill(pid,SIGUSR1);
	//kill(p2,SIGUSR2);
	exit(0); 
}
int main(int argc,char *argv[]){
	if(argc<2){
		printf("arguement missing!!\n");
		exit(0);
	}
	port_num = atoi(argv[1]);
	int shmid = init(shmkey);
	int sid = init(atoi(argv[1]));
	pr = (int*)shmat(sid,NULL,0);

	struct sockaddr_in serv_addr;
	struct hostent *serv;
	ptr = (int *)shmat(shmid,NULL,0);
	 sfd = socket(AF_INET,SOCK_STREAM,0);
	signal(SIGINT,sighandler);
	if(sfd<0)
	{
		printf("socket error!!\n");
		exit(0);
	}	
    serv = gethostbyname("localhost");
    if(serv==NULL){
    	printf("gethostbyname error!!\n");
    	exit(0);
    }

    bcopy((char *)serv->h_addr,(char*)&serv_addr.sin_addr.s_addr,serv->h_length);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port= htons(portno);
    printf("%d\n",portno);
    if(connect(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    	printf("connect error!!\n");
    	
    	exit(0);
    }
	char wbuffer[256],temp[256];
	bzero(temp,256);
	//temp[0]='.';temp[1]='/';temp[2]='s';temp[3]=argv[1][0];
	//printf("%s\n",temp);
	send(sfd,argv[1],strlen(argv[1]),0);
	recv(sfd,temp,256,0);
	if(strcmp(temp,"accept")==0)
	{
	printf("Connection succesful\n");
	shutdown(sfd,SHUT_RDWR);
	sleep(1);
	sfd = socket(AF_INET,SOCK_STREAM,0);
	bzero((char *) &serv_addr,sizeof(serv_addr));
	 serv = gethostbyname("localhost");
    if(serv==NULL){
    	printf("gethostbyname error!!\n");
    	exit(0);
    }

    bcopy((char *)serv->h_addr,(char*)&serv_addr.sin_addr.s_addr,serv->h_length);
    
	serv_addr.sin_family = AF_INET;
	printf("%d\n",atoi(argv[1]));
	serv_addr.sin_port = htons(atoi(argv[1]));//htons  converts a port number in host byte order to a port number in network byte order
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	 if(connect(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    	printf("connect error!!\n");
    	raise(SIGINT);
    	exit(0);
    }
	//bzero((char *) &serv_addr,sizeof(serv_addr));
    char buf[256];
    bzero(buf,256);
    sprintf(buf,"%s","I am client 50005");

    send(sfd,buf,strlen(buf),0);
    while(1){
	bzero(buf,256);
	scanf("%s",buf);
	send(sfd,buf,strlen(buf),0);
	//pthread_t pread,pwrite;
		}
}
else{
	printf("sorry server is full!!\n");
}
}	