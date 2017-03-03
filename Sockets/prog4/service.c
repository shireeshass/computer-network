#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
int nsfd[50];
int num=0;
int maxfd =0;
fd_set readfd;
void *sendsmsg(void *nm){
	int *nfd = (int*)nm;
	while(1){
		char buf[256];
		fgets(buf,256,stdin);
		send(nfd[*nfd],buf,strlen(buf),0);	
	}
}
void *recvsmsg(void *nm){
	int *nfd = (int*)nm;
	int in=*nfd;
	printf("%d  %d\n",in,maxfd);
	while(1){
		FD_CLR(nsfd[in],&readfd);
		FD_SET(nsfd[in],&readfd);
		int ret = select(maxfd+1,&readfd,NULL,NULL,NULL);
		if(ret>0 && FD_ISSET(nsfd[in],&readfd)){
		//printf("shit\n");
		char buf[256];
		bzero(buf,256);
		if(recv(nsfd[in],buf,256,0)==0){
			nsfd[in] = 0;
			FD_CLR(nsfd[in],&readfd);
			printf("Thread Exitting\n");
			fflush(stdout);
			pthread_exit(NULL);			
		}
		int i=0;
		printf("%s\n",buf);
		for(i=0;i<num;i++)
		{
			if(nsfd[i]!=0)
			send(nsfd[i],buf,strlen(buf),0);
		}	
	}
	}
}
int shmsize = 10;


int main(int argc,char *argv[1]){
	if(argc<2){
		printf("arguements missing !!\n");
		exit(0);
	}
	//signal(SIGUSR2,handler);
	struct sockaddr_in serv_addr,cli_addr;
	int sfd,i;
	
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0){
		printf("sfd error!!\n");
		exit(0);
	}
	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//printf("%d\n",portno.porno);
	serv_addr.sin_port = htons(atoi(argv[1]));//htons  converts a port number in host byte order to a port number in network byte order
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
	{
		printf("bind Error!!\n");
		exit(0);
	}
	pthread_t pread[20];
	listen(sfd,5);
	while(1){
		int cllength = sizeof(cli_addr);
	    nsfd[num] = accept(sfd,(struct sockaddr*)&cli_addr,&cllength);
		if(nsfd[num]>maxfd)
			maxfd = nsfd[num];
		
		printf("accepted by service\n");
		if(nsfd[num] < 0){
			printf("accept error!!\n");
			exit(0);
		}
		int nx = num;
		FD_SET(nsfd[nx],&readfd);
		pthread_create(&pread[nx],NULL,recvsmsg,(void*)&nx);
		//pthread_create(&pwrite,NULL,sendsmsg,(void*)&nx);
		
		num++;
		printf("num is %d\n",num);
	}
	
}