#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <poll.h>
#include <string.h>
main(int argc,char *argv[])
{
	mkfifo("ms",0666);
	int fd = open("ms",O_RDONLY);
	int i=1;
	char sbuf[32]="./serv";
	char buf[32];
	int servernum;
	char sname[2];
	int stat;
	int pid;
	while(1)
	{
	char num[5];
	read(fd,num,5);
	sprintf(buf,"%s%s", sbuf,num);
	printf("%s\n",buf);
	printf("request cumming\n");
	char ws[5];ws[0]='w';ws[1]='s';ws[2]=num[0];ws[3]='\0';
	char rs[5];rs[0]='r';rs[1]='s';rs[2]=num[0];rs[3]='\0';
	printf("yes\n");
	if(mkfifo(ws,0666) == -1){
		printf("Error service busy\n");
	}
	if(mkfifo(rs,0666) == -1){
		printf("Error service busy\n");
	}	
	printf("helllllooooo\n");
			
			int fr = open(rs,O_RDONLY);
			if(fr == -1){
				printf("RERROR\n");
			}
			sleep(2);
			int fw = open(ws,O_WRONLY);
			if(fw == -1) {
				printf("WERROR\n");
			}
		
		printf("okk\n");
		pid  = fork();
		if(pid == 0)
		{
			
				dup2(fw,1);
				dup2(fr,0);

				printf("%s\n",buf);
				sleep(2);
				printf("wakeup\n");
				
				execvp(buf,argv);
				printf("Errrrrror\n");
		}

		
	}
}