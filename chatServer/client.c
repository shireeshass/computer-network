#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
int timeout_msecs =500;
int flag =0;
int f,fs;
char ar[512];
void *readfifo(void *num){
		while(1){
			
				char buf[1024];
				//printf("before\n");
				if(read(f,buf,1024) <=0){
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
      	
      	write(fs,buf,sizeof(buf));
      } 
}
int main(int argc,char *argv[]){
	if(argc <2)
	{
		printf("arguement missing!! \n");
		exit(0);
	}
	strcpy(ar,argv[1]);
	fs = open("cs",O_WRONLY);
	printf("%s\n",argv[1]);
	f = open(argv[1],O_RDONLY);
	
	
	pthread_t rthread,wthread;
	pthread_create(&rthread,NULL,readfifo,(void*)0);
	pthread_create(&wthread,NULL,writefifo,(void*)0);
	pthread_join(rthread,NULL);
	pthread_join(wthread,NULL);
}