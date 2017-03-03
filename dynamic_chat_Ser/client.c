#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
int fw,fr;
void *readfifo(void *num){
			while(1){
				char buf[2048];
				read(fr,buf,2048);
				fprintf(stdout,"%s\n",buf);
			}
}
void *writefifo(void *num){
	while(1){
		char buf[1024];
		fscanf(stdin,"%s",buf);
		write(fw,buf,1024);
		sleep(0.5);
	}
}
main(int argc,char *argv[])
{
	printf("Enter Service number u want to connect to ... : ");
	char num[5];
	scanf("%s",num);
	int fd = open("ms",O_WRONLY);
	printf("ms openned\n");
	write(fd,num,5);
	printf("msg sent to main server\n");
	char ws[5];ws[0]='w';ws[1]='s';ws[2]=num[0];ws[3]='\0';
	char rs[5];rs[0]='r';rs[1]='s';rs[2]=num[0];rs[3]='\0';
	printf("ok report  %s  %s",ws,rs);
	
	sleep(1);
	if((fw = open(rs,O_WRONLY))==-1){
		printf("error rs!!\n");exit(0);
	}else{
		printf("ok1\n");
	}
	
	sleep(1);
	if((fr = open(ws,O_RDONLY))==-1){
		printf("error ws!!\n");exit(0);
	}
	else{
		printf("ok2\n");
	}
	
	printf("working\n");
	pthread_t pread,pwrite;
	pthread_create(&pread,NULL,readfifo,(void*)0);
	pthread_create(&pwrite,NULL,writefifo,(void*)0);
	pthread_join(pread,NULL);
	pthread_join(pwrite,NULL);
}