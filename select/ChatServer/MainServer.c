#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>
int timeout_msecs =500;
main(){
	int fdw[4],fdr[4];
	 int fd;
	 struct timeval tm;
	 tm.tv_sec = 2;
	 tm.tv_usec = timeout_msecs;
	fd_set readfd;
	mkfifo("cr1",0666);
	mkfifo("cr2",0666);
	mkfifo("cr3",0666);
	mkfifo("cr4",0666);
	mkfifo("cw1",0666);
	mkfifo("cw2",0666);
	mkfifo("cw3",0666);
	mkfifo("cw4",0666);
	//fd = open("cs",O_RDONLY);
	fdw[0] = open("cw1",O_WRONLY);
	fdr[0] = open("cr1",O_RDONLY);
	fdw[1] = open("cw2",O_WRONLY);
	fdr[1] = open("cr2",O_RDONLY);
	fdw[2] = open("cw3",O_WRONLY);
	fdr[2] = open("cr3",O_RDONLY);
	fdw[3] = open("cw4",O_WRONLY);
	fdr[3] = open("cr4",O_RDONLY);
	//char *crlist[]={"cr1","cr2","cr3","cr4"};
	//char *cwlist[]={"cw1","cw2","cw3","cw4"};
	int i=0;
	int j=0;

	char buf[1024];
	//printf("%d\n",maxfd);
	printf("working\n");
	while(1){
		int maxfd=0;
	FD_ZERO(&readfd);
	for(i=0;i<4;i++)
	{
		FD_SET(fdr[i],&readfd);
		printf("%d\n",fdr[i]);
		maxfd = (maxfd > fdr[i]) ? maxfd : fdr[i];
	}
		int ret = select(maxfd+1,&readfd,NULL,NULL,NULL);
		
		if(ret>0){
		
		printf("ok\n");
			for(i=0;i<4;i++)
			{
				if(FD_ISSET(fdr[i],&readfd))
				{
					read(fdr[i],buf,1024);
						for(j=0;j<4;j++)
						{
							printf("yes\n");
							if(j!=i)
							write(fdw[j],buf,1024);
						}
					
				}
				//FD_CLR(fdr[i],&readfd);
			}

		//fprintf(stdout,"%s",buf);
	
		
	}
	else if(ret ==-1)
	{
		printf("select error!!\n");
		exit(0);
	}

	}
}