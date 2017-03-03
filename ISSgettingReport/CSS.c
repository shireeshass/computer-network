/*
	Author :- Vinay Kant
	ISS getting report from server process
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
/*
struct sockaddr_in{
	short sin_family; must be AF_INET
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8]; Not used , must be zero	
};
*/
struct portsinfo{
	int porno;
	int status;
	char name[10];
};
int main(){
	
	struct sockaddr_in serv_addr[5],cli_addr;
	int sfd[5];
	FILE *in;
	fd_set fdread;
	in = fopen("report.txt","r");
	char line[256];
	struct portsinfo portno[5];
	int l=0;
	while(fgets(line,256,in)){
		int i=0,j=0;
		char temp[10];
		while(line[i]!='|')
		{
			temp[j] = line[i];
			i++;j++;
		}
		temp[j]='\0';
		j=0; 
		portno[l].porno = atoi(temp);
		i++;
		bzero(temp,10);
		while(line[i]!='|')
		{	
			temp[j] = line[i];
			i++;j++;
		}
		temp[j]='\0';
		j=0;
		portno[l].status = atoi(temp);
		i++;
		bzero(temp,10);
		while(line[i]!='#')
		{
			portno[l].name[j]=line[i];
			i++;j++;
		}
		portno[l].name[j]='\0';
		
		//printf("%d  %d  %s\n",portno[l].porno,portno[l].status,portno[l].name);
		l++;
		bzero(line,256);
	}
	int i=0;
	for(i=0;i<5;i++){
		sfd[i] = socket(AF_INET,SOCK_STREAM,0);
		if(sfd[i]<0)
		{
			printf("socket error!!\n");
			exit(0);
		}
	}
		
	// to initclose(nsfd);ialie the server address
	for(i=0;i<5;i++){
	bzero((char *) &serv_addr[i],sizeof(serv_addr[i]));
	serv_addr[i].sin_family = AF_INET;
	printf("%d\n",portno[i].porno);
	serv_addr[i].sin_port = htons(portno[i].porno);//htons  converts a port number in host byte order to a port number in network byte order
	serv_addr[i].sin_addr.s_addr = INADDR_ANY;
	if(bind(sfd[i],(struct sockaddr *) &serv_addr[i],sizeof(serv_addr[i]))<0)
	{
		printf("bind Error!!\n");
		exit(0);
	}
	listen(sfd[i],3);
}
	char rbuffer[256],temp[256];

	
	while(1){
		FD_ZERO(&fdread);
		int maxfd=0;
		for(i=0;i<5;i++)
		{
			FD_SET(sfd[i],&fdread);
			maxfd = (maxfd>sfd[i])?maxfd:sfd[i];
		}
		int ret = select(maxfd+1,&fdread,NULL,NULL,NULL);
		if(ret>0){
			printf("hello\n");
			int j=0;
		for(j=0;j<5;j++)
		{
			if(FD_ISSET(sfd[j],&fdread))
				break;
		}
		bzero((char *)&cli_addr,sizeof(cli_addr));
		int cllength = sizeof(cli_addr);

		int nsfd = accept(sfd[j],(struct sockaddr *)&cli_addr,&cllength);
		if(nsfd < 0){
			printf("accept error!!\n");
			exit(0);
		}
		//
		if(fork()==0){
		for(l=0;l<5;l++)
			close(sfd[l]);
		dup2(nsfd,0);
		dup2(nsfd,1);
		sleep(1);
		//printf("%s\n",portno[j].name);
		execlp(portno[j].name,portno[j].name,NULL);
		printf("exec error!!\n");
		}
			close(nsfd);
		}
		
	}
		//printf("shit\n");*/
}	