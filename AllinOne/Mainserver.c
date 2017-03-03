#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <signal.h>
#include <fcntl.h>
#define shmkey  1234
#define shmsize 4
int opfd;
void handler(){
	//printf("Signal -- Hi\n");
	write(opfd,"Signal recieved",sizeof("Signal recieved"));
}
int  init(int key){
	int shmid = shmget(key,shmsize,IPC_CREAT|0666);
	if(shmid <=0){
		printf("shmget error!!\n");
		exit(0);
	}
	return shmid;
}
main(){
	int pfd[2];
	int shmid = init(shmkey);
	signal(SIGUSR1,handler);
	int *ppid = (int *)shmat(shmid,NULL,0);
	*ppid = getpid();
	mkfifo("sfifo",0666);
	struct pollfd fds[3];
	int ffd = open("sfifo",O_RDONLY);
	pipe(pfd);	
	fds[0].fd = pfd[0];
	fds[1].fd = ffd;
	mkfifo("op",0666);
	opfd = open("op",O_WRONLY);
	FILE *f;
	f = popen("./spopen","r");
	fds[2].fd = fileno(f);
	//fds[3].fd = fileno(stdin);
	//fds[3].events = POLLIN;
	int i=0;
	for(i=0;i<3;i++){
		fds[i].events = POLLIN;
	} 
	
	int pid = fork();
	if(pid == -1){
		printf("fork error\n");
	}
	printf("yes\n");
	if(pid == 0){
   		close(pfd[0]);
   		dup2(pfd[1],1);
   		printf("exec\n");
   		execlp("./spipe","./spipe",NULL);
   		printf("exec error\n");
	}
	else{
		
		while(1){
			
			int ret = poll(fds,3,500);
			if(ret>0){
				for( i=0;i<3;i++){
					if(fds[i].revents & POLLIN){
						break;
					}
				}
				printf("%d\n",i);
				char buf1[1024];
				if(i==0){
					sprintf(buf1,"%s","pipe: ");
				}
				else if(i==1){
					sprintf(buf1,"%s","fifo: ");
				}
				else if(i==2){
					sprintf(buf1,"%s","popen: ");
				}
				
				char buf[1024];
				read(fds[i].fd,buf,1024);
				char outpu[1024];
				sprintf(outpu,"%s%s",buf1,buf);
				write(opfd,outpu,1024);
			}
		}
	}

}
