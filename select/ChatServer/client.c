#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
int timeout_msecs =500;
int flag =0;
int fsr,fsw;
char ar[512];
void *readfifo(void *num){
		while(1){
			
				char buf[1024];
				//printf("before\n");
				if(read(fsr,buf,1024) <=0){
					sleep(0.5);
					printf("read error\n");
				}
				//printf("after\n");
				fprintf(stdout,"%s\n",buf);
		}
}
void *writefifo(void *num){
      
      while(1){
      	
      	char buf[512];
      	char b[512] ;
      	strcpy(buf,ar);
      	strcat(buf,":-");
      	fscanf(stdin,"%s",b);
      	strcat(buf,b);
      	
      	write(fsw,buf,sizeof(buf));
      } 
}
int main(int argc,char *argv[]){
	if(argc <2)
	{
		printf("arguement missing!! \n");
		exit(0);
	}
	char cw[4];
	cw[0]='c';cw[1]='r';cw[2]=argv[1][0];cw[3]='\0';
	char cr[4];
	cr[0]='c';cr[1]='w';cr[2]=argv[1][0];cr[3]='\0';
	
	ar[0]='c';ar[1]=argv[1][0];ar[2]='\0';
	fsr = open(cr,O_RDONLY);
	fsw = open(cw,O_WRONLY);
	printf("%s   %s\n",cr,cw);
	pthread_t rthread,wthread;
	pthread_create(&rthread,NULL,readfifo,(void*)0);
	pthread_create(&wthread,NULL,writefifo,(void*)0);
	pthread_join(rthread,NULL);
	pthread_join(wthread,NULL);
}
