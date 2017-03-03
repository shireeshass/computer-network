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
struct sockaddr_in ser_addr;
int portno,ser_len;

void* snd_msg(void* arg) // reads from keyboard and sends to server
{
	struct message msg;
	msg.cno = cno;
	while(1)
	{
		memset(msg.buf,0,100);
		read(0,msg.buf,100);
		msg.length = strlen(msg.buf);
		if(sendto(sfd,&msg,sizeof(struct message),0,(struct sockaddr*)(&ser_addr),ser_len)<0)
			derror("Send error");
	}
}

void* rcv_msg(void* arg) // receives from server and displays on screen
{
	struct message msg;
	msg.cno = cno;
	while(1)
	{
		memset(msg.buf,0,100);
		if(recvfrom(sfd,&msg,sizeof(struct message),0,(struct sockaddr*)(&ser_addr),&ser_len)<0)
			derror("receive error");
		write(1,msg.buf,msg.length);
	}
}



int main(int argc,char* args[])
{
	pthread_t sender,receiver;
	
	sfd = socket(AF_INET,SOCK_DGRAM,0); // socket create... AF_INET for IPv4 domain and SOCK_DGRAM for connection less systems
	
	if(sfd < 0)
		derror("Socket create");

	memset(&ser_addr,0,sizeof(struct sockaddr_in)); // Initialize to 0

	ser_addr.sin_family = AF_INET;
	portno = atoi(args[1]);
	ser_addr.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	ser_addr.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine
	ser_len = sizeof(struct sockaddr_in);


	pthread_create(&sender,NULL,snd_msg,NULL); // for sending msg to server
	pthread_create(&receiver,NULL,rcv_msg,NULL); // for rcving msg from server

	pthread_join(sender,NULL);

	return 0;
}