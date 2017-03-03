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

int sfd,lsfd[10],noc,cli_len; // lsfd holds fd of each client and noc hold no of clients 

struct message // structure of message to be sent
{
	char buf[100];
	int cno;
	int length;
};

struct node  // structure for holding client addresses
{
	struct sockaddr_in addr;
	int cno;
	struct node *next;
};

void add_end(struct node *T,struct sockaddr_in cli_addr)
{
	if(T->next!=NULL)
		add_end(T->next,cli_addr);

	else
	{
		T->next = malloc(sizeof(struct node));
		T = T->next;
		T->addr = cli_addr;
		T->cno = ++noc;
		T->next = NULL;
	}
	return;
}

int find_node(struct node *T,struct sockaddr_in cli_addr) // find if node ios in buf
{
	if (T==NULL)
	{
		return 0;
	}
	else if(T->addr.sin_addr.s_addr == cli_addr.sin_addr.s_addr && T->addr.sin_port == cli_addr.sin_port)
	{
		return T->cno;
	}
	else 
	{
		return find_node(T->next,cli_addr);
	}
}

void send_msg(struct node *T,struct message* msg,int no)
{
	if(T!=NULL)
	{
		if(T->cno!=no)
			sendto(sfd,msg,sizeof(struct message),0,(struct sockaddr*)&(T->addr),cli_len);
		send_msg(T->next,msg,no);
	}
}

int main(int argc,char* args[])
{
	struct sockaddr_in ser_addr,cli_addr;
	int portno,i,j,max; // max holds max fd value + 1
	struct message msg;
	struct node *T;
	T = NULL;
	noc = 0;
	
	sfd = socket(AF_INET,SOCK_DGRAM,0); // socket create... AF_INET for IPv4 domain and SOCK_STREAM for connection oriented systems
	
	if(sfd < 0)
		derror("Socket create");

	memset(&ser_addr,0,sizeof(struct sockaddr_in)); // Initialize to 0

	ser_addr.sin_family = AF_INET;
	portno = atoi(args[1]);
	ser_addr.sin_port = htons(portno); // converts int to 16 bit integer in network byte order
	ser_addr.sin_addr.s_addr = INADDR_ANY; // to get IP address of machine on which server is running

	if(bind(sfd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr_in))<0)
		derror("Bind error");

	cli_len = sizeof(struct sockaddr_in);

	while(1)
	{
		memset(&cli_addr,0,sizeof(struct sockaddr_in)); // clear cli_addr  everytime
		i = recvfrom(sfd,&msg,sizeof(struct message),0,(struct sockaddr*)&cli_addr,&cli_len);
		j = errno;
		if(i!=-1)
		{
			if((i=(find_node(T,cli_addr)))>0)
			{
				printf("%s",msg.buf);
				send_msg(T,&msg,i);
			}
			else
			{
				printf("Ta-da!! Welcome client %d\n",noc+1);
				if(noc>0)
				add_end(T,cli_addr);
				else
				{
					T = malloc(sizeof(struct node));
					T->addr = cli_addr;
					T->cno = ++noc;
					T->next = NULL;
				}
			}
			//printf("%d\n",T->cno);
			//printf("1\n");
		}
		 else
		{
			printf("%d",j);
			derror("Receive error");
		}
	}

	return 0;
}
