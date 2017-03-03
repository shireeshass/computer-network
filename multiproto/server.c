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
#define derror(msg) {printf("%s\n", msg); exit(0);}

int csfd,lsfd,ncsfd; // csfd holds fd of each client and noc hold no of clients 

struct message // structure of message to be sent
{
	char buf[100];
	int cno;
	int length;
};

int main(int argc,char* args[])
{
	struct sockaddr_in ser_addr1,ser_addr2,cli_addr;
	int portno,cli_len,i,j,max; // max holds max fd value + 1
	char buf[256];
	fd_set readfds;
	
	csfd = socket(AF_INET,SOCK_STREAM,0); // socket create... AF_INET for IPv4 domain and SOCK_STREAM for connection oriented systems
	lsfd = socket(AF_INET,SOCK_DGRAM,0);

	cli_len = sizeof(struct sockaddr_in);
	
	if(csfd < 0)
		derror("Socket create");

	memset(&ser_addr1,0,sizeof(struct sockaddr_in)); // Initialize to 0
	memset(&ser_addr2,0,sizeof(struct sockaddr_in));

	// address of connection oriented socket
	ser_addr1.sin_family = AF_INET;
	portno = atoi(args[1]);
	ser_addr1.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	ser_addr1.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine on which server is running

	// address of connection less socket
	ser_addr2.sin_family = AF_INET;
	portno = 12723;
	ser_addr2.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	ser_addr2.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine on which server is running

	if(bind(csfd,(struct sockaddr*)&ser_addr1,sizeof(struct sockaddr_in))<0)
		derror("Bind error");
	if(bind(lsfd,(struct sockaddr*)&ser_addr2,sizeof(struct sockaddr_in))<0)
		derror("Bind error");
	
	if(listen(csfd,10)<0) // No. of clients that server can service
		derror("Listen error");

	cli_len = sizeof(struct sockaddr_in);

	struct timespec tim_dur;
	tim_dur.tv_sec = 1;   // waiting time of 1 sec for verifying set bits of FDSET
	tim_dur.tv_nsec = 0;

	max=csfd; // find maximum file descriptor
	if(lsfd>max)
		max=lsfd;

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(csfd,&readfds); // Set the connection oriented sfd
		FD_SET(lsfd,&readfds); // Set the connection less sfd

		// pselect for finding if data is available
		//printf("HEllo\n");
		if(pselect(max+1,&readfds,NULL,NULL,&tim_dur,NULL)>0); // pselect is used as timeleft is not updated after each select call
		{
			//printf("Select truee\n");
			if(FD_ISSET(csfd,&readfds))
			{
				printf("COS");
				ncsfd = accept(csfd,(struct sockaddr*)&cli_addr,&cli_len);
				int c = fork();
				if(c>0)
				{
					close(ncsfd);
					wait(NULL);
				}
				else
				{
					close(csfd);
					dup2(ncsfd,0);
					execlp("./s1","s1",(char*) 0); // capitalise the buffer
					exit(0);
				}
			}
			else
			{
				printf("CLS");
				memset(buf,0,256);
				if(recvfrom(lsfd,buf,256,MSG_DONTWAIT,(struct sockaddr*)&cli_addr,&cli_len)>0)
				{
					for(i=0;i<strlen(buf);i++) // convert buffer to lower case
						buf[i] = tolower(buf[i]);
					sendto(lsfd,buf,256,0,(struct sockaddr*)&cli_addr,cli_len);
				}
			}
		}
		//printf("Some testing\n");
	}

	return 0;
}