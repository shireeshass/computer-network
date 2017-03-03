/*
	Author :- Vinay Kant
	Concurrent Connection Oriented service
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define portno 5000
/*
struct sockaddr_in{
	short sin_family; must be AF_INET
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8]; Not used , must be zero	
};
*/
int main(){
	
	struct sockaddr_in serv_addr,cli_addr;
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		printf("socket error!!\n");
		exit(0);
	}	
	// to initclose(nsfd);ialie the server address
	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);//htons  converts a port number in host byte order to a port number in network byte order
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
	{
		printf("bind Error!!\n");
		exit(0);
	}
	char rbuffer[256],temp[256];

	listen(sfd,5);
	while(1){
		
		int cllength = sizeof(cli_addr);
		int nsfd = accept(sfd,(struct sockaddr *)&cli_addr,&cllength);
		if(nsfd < 0){
			printf("accept error!!\n");
			exit(0);
		}
		char name[5];
		recv(nsfd,name,5,0);
		//printf("%s\n",name);
		if(fork()==0){
		bzero(rbuffer,256);
		bzero(temp,256);
		close(sfd);
		dup2(nsfd,0);
		dup2(nsfd,1);
		sleep(1);
		execlp(name,name,NULL);
		printf("exec error!!\n");
		}
			close(nsfd);
		
	}
		//printf("shit\n");
}	