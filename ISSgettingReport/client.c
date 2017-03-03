/*
	Author :- Vinay Kant
	Iterative Connection Oriented service
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>	
#include <netdb.h>
/*
struct sockaddr_in{
	short sin_family; must be AF_INET
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8]; Not used , must be zero	
};
*/
int main(int argc,char *argv[]){
	if(argc<2){
		printf("arguement missing!!\n");
		exit(0);
	}
	struct sockaddr_in serv_addr;
	struct hostent *serv;
	
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	int portno;
	portno = atoi(argv[1]);
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
    if(connect(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    	printf("connect error!!\n");
    	exit(0);
    }
	char wbuffer[256],temp[256];
	
	//printf("%s\n",temp);
	//send(sfd,temp,strlen(temp),0);
	printf("Connection succesful\n");
	
	fgets(wbuffer,255,stdin);
	if(send(sfd,wbuffer,strlen(wbuffer),0)<0){
		printf("write error!!\n");
		exit(0);
	}
	bzero(temp,256);
	if(recv(sfd,temp,255,0)<0){
		printf("error!!\n");
		exit(0);
	}
	printf("%s\n", temp);

}	