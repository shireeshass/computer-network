#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

main(int argc,char *argv[]){
	char buf[1024];
	char fname[32]="msfifo";
	char name[32] ; sprintf(name,"%s%s",fname,argv[1]);
     int fd = open(name,O_WRONLY);	
	if(fork()==0)
	{
		
			printf("order\n");
			memset(buf,1024,'\0');
			//sleep(3);
			//scanf("%s",buf);
			sprintf(buf, "ps");
			//sleep(1);
			printf("buffer is %s\n",buf);
			//sleep(1);
			dup2(fd,1);
			execlp(buf, buf, NULL);
			printf("Error execv\n");	
	}
	else{	
		wait(0);
	}

}