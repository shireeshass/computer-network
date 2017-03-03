#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

main(){
	char buf[1024];
	
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
			execlp(buf, buf, NULL);
			printf("Error execv\n");
		
		
	}
	else{	
		wait(0);
	}

}