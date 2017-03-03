#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <fcntl.h>
int timeout_msecs =500;
main(){
	int fds[4];
	struct pollfd fser ; int fd;
	mkfifo("cs",0666);
	mkfifo("c1",0666);
	mkfifo("c2",0666);
	mkfifo("c3",0666);
	mkfifo("c4",0666);
	fd = open("cs",O_RDONLY);
	fds[0] = open("c1",O_WRONLY);
	fds[1] = open("c2",O_WRONLY);
	fds[2] = open("c3",O_WRONLY);
	fds[3] = open("c4",O_WRONLY);
	fser.events = POLLIN;
	char *clist[]={"c1","c2","c3","c4"};
	int i=0;
	int j=0;
	char buf[1024];
	printf("working\n");
	while(1){
		//int ret = poll(&fser,1,timeout_msecs);
		
		//if(ret>0){
		
		//printf("ok%d\n",i);
		if(read(fd,buf,1024)<=0)
			printf("error!!\n");
		else{
			//printf("okk\n");
		}

		fprintf(stdout,"%s",buf);
		for(j=0;j<4;j++)
		{
				//printf("yes\n");
				
				write(fds[j],buf,1024);
			
		}
		
	//}
	}
}