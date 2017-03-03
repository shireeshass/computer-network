#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define derror(msg) {printf("%s\n", msg); exit(0);}

struct message
{
	char buf[100];
	int cno;
	int length;
};


int sfd,cfd;


int main(int argc,char* args[])
{
	struct sockaddr_in ser_addr,my_addr,cli_addr;
	int portno,cli_len,i;
	
	sfd = socket(AF_INET,SOCK_STREAM,0); // socket create... AF_INET for IPv4 domain and SOCK_STREAM for connection oriented systems
	
	if(sfd < 0)
		derror("Socket create");

	memset(&ser_addr,0,sizeof(struct sockaddr_in)); // Initialize to 0

	ser_addr.sin_family = AF_INET;
	portno = atoi(args[2]);
	ser_addr.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	ser_addr.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine on which server is running

	my_addr.sin_family = AF_INET;
	portno = atoi(args[1]);
	my_addr.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	printf("%d\n", (int)my_addr.sin_port);
	my_addr.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine on which server is running

	// if(bind(sfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr_in))<0)
	// 	derror("Bind error");

	int true = 1;
	if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int))<0)
		derror("Reuse error");

	if(connect(sfd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr_in)) < 0)
		derror("Connect error"); // connect to server

	send(sfd,&my_addr,sizeof(struct sockaddr_in),0);
	close(sfd);
	sleep(1);

	sfd = socket(AF_INET,SOCK_STREAM,0); // socket create... AF_INET for IPv4 domain and SOCK_STREAM for connection oriented systems
	if(sfd<0)
		derror("Socket create");



	if(bind(sfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr_in))<0)
	{
		i=errno;
		printf("%d\n",i);
		derror("Bind error");
	}

	
	if(listen(sfd,10)<0) // No. of clients that server can service
		derror("Listen error");

	cli_len = sizeof(struct sockaddr_in);

	cfd = accept(sfd,(struct sockaddr*)&cli_addr,&cli_len);
	char buf[256];
	while(1)
	{
		memset(buf,0,256);
		read(0,buf,256);
		send(cfd,buf,256,0);
		memset(buf,0,256);
		while(recv(cfd,buf,256,0)<=0);
		write(1,buf,256);
	}

	// true = 1;
	// setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));

	/*while(recv(sfd,buf,256,0)<0);

	write(1,buf,strlen(buf));*/

	// pthread_create(&sender,NULL,snd_msg,NULL); // for sending msg to server
	// pthread_create(&receiver,NULL,rcv_msg,NULL); // for rcving msg from server

	// pthread_join(sender,NULL);

	return 0;
}