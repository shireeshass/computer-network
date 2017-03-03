#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define derror(msg) {printf("%s\n", msg); exit(0);}

int sfd,cfd,c1fd; // csfd holds fd of each client and noc hold no of clients 

struct message // structure of message to be sent
{
	char buf[100];
	int cno;
	int length;
};

// here client becomes ser1!!

int main(int argc,char* args[])
{
	struct sockaddr_in ser_addr,cli_addr,ser1_addr;
	int portno,cli_len,i,j,max; // max holds max fd value + 1
	struct message msg;
	
	sfd = socket(AF_INET,SOCK_STREAM,0); // socket create... AF_INET for IPv4 domain and SOCK_STREAM for connection oriented systems
	
	if(sfd < 0)
		derror("Socket create");

	memset(&ser_addr,0,sizeof(struct sockaddr_in)); // Initialize to 0
	memset(&cli_addr,0,sizeof(struct sockaddr_in));
	memset(&ser1_addr,0,sizeof(struct sockaddr_in));

	ser_addr.sin_family = AF_INET;
	portno = atoi(args[1]);
	ser_addr.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	ser_addr.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine on which server is running

	if(bind(sfd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr_in))<0)
		derror("Bind error");
	
	if(listen(sfd,10)<0) // No. of clients that server can service
		derror("Listen error");

	cli_len = sizeof(struct sockaddr_in);

	cfd = accept(sfd,(struct sockaddr*)&ser1_addr,&cli_len);

	while(recv(cfd,&ser1_addr,cli_len,0)<=0);
	printf("%d\n", (int)ser1_addr.sin_port);

	close(cfd);
	close(sfd);

	//connecting to c using c1fd

	c1fd = socket(AF_INET,SOCK_STREAM,0); // socket create... AF_INET for IPv4 domain and SOCK_STREAM for connection oriented systems
	
	if(c1fd < 0)
		derror("Socket create");

	//memset(&ser1_addr,0,sizeof(struct sockaddr_in)); // Initialize to 0
	sleep(4);


	if(connect(c1fd,(struct sockaddr*)&ser1_addr,sizeof(struct sockaddr_in)) < 0)
	{
		i=errno;
		printf("%d\n",i);
		derror("Connect error"); // connect to server
	}

	char buf[256];

	while(1)
	{
		memset(buf,0,256);
		while(recv(c1fd,buf,256,0)<=0);
		for(i=0;i<strlen(buf);i++)
			buf[i]=toupper(buf[i]);
		send(c1fd,buf,256,0);
	}

	return 0;
}
