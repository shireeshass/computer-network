#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
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
	fd_set readfd;

	//struct pollfd
	int fds[3];
	int ffd = open("sfifo",O_RDONLY);
	pipe(pfd);	
	fds[0] = pfd[0];
	fds[1] = ffd;
	mkfifo("op",0666);
	opfd = open("op",O_WRONLY);
	FILE *f;
	f = popen("./spopen","r");
	fds[2] = fileno(f);
	//fds[3].fd = fileno(stdin);
	//fds[3].events = POLLIN;
	int i=0;

	int flg=0;
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
			int maxfd=0;
	for(i=0;i<3;i++){
		FD_SET(fds[i],&readfd);
		maxfd = (maxfd>fds[i])?maxfd:fds[i];
		//fds[i].events = POLLIN;
	} 
	     if(flg)
		FD_CLR(fds[2],&readfd);
			int ret = select(maxfd+1,&readfd,NULL,NULL,NULL);
			if(ret>0){
				for( i=0;i<3;i++){
					if(FD_ISSET(fds[i],&readfd)){
						char buf1[1024];
				if(i==0){
					sprintf(buf1,"%s","pipe: ");
				}
				else if(i==1){
					sprintf(buf1,"%s","fifo: ");
				}
				else if(i==2){
					flg=1;
					sprintf(buf1,"%s","popen: ");
				}
				
				char buf[1024];
				read(fds[i],buf,1024);
				char outpu[1024];
				sprintf(outpu,"%s%s",buf1,buf);
				write(opfd,outpu,1024);
				
					}
				}
			//	printf("%d\n",i);
				
			}
		}
	}

}
