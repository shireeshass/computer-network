#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#define derror(msg) {printf("%s\n", msg); exit(0);}

struct message
{
	char buf[100];
	int cno;
	int length;
};


int sfd,cno;

void* snd_msg(void* arg) // reads from keyboard and sends to server
{
	struct message msg;
	msg.cno = cno;
	while(1)
	{
		memset(msg.buf,0,100);
		read(0,msg.buf,100);
		msg.length = strlen(msg.buf);
		send(sfd,&msg,sizeof(struct message),0);
	}
}

void* rcv_msg(void* arg) // receives from server and displays on screen
{
	struct message msg;
	msg.cno = cno;
	while(1)
	{
		memset(msg.buf,0,100);
		while(recv(sfd,&msg,sizeof(struct message),0)<0);
		write(1,msg.buf,msg.length);
	}
}



int main(int argc,char* args[])
{
	struct sockaddr_in ser_addr;
	int portno;
	pthread_t sender,receiver;
	
	sfd = socket(AF_INET,SOCK_STREAM,0); // socket create... AF_INET for IPv4 domain and SOCK_STREAM for connection oriented systems
	
	if(sfd < 0)
		derror("Socket create");

	memset(&ser_addr,0,sizeof(struct sockaddr_in)); // Initialize to 0

	ser_addr.sin_family = AF_INET;
	portno = atoi(args[1]);
	ser_addr.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	ser_addr.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine on which server is running

	if(connect(sfd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr_in)) < 0)
		derror("Connect error"); // connect to server

	/*while(recv(sfd,buf,256,0)<0);

	write(1,buf,strlen(buf));*/

	pthread_create(&sender,NULL,snd_msg,NULL); // for sending msg to server
	pthread_create(&receiver,NULL,rcv_msg,NULL); // for rcving msg from server

	pthread_join(sender,NULL);

	return 0;
}