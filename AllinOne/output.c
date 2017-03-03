#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
main(){
	int fd  = open("op",O_RDONLY);
	while(1){
		char buf[1024];
		read(fd,buf,1024);
		printf("%s\n",buf);
		sleep(2);
	}
}