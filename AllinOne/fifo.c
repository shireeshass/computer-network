#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
main(){
	int fd = open("sfifo",O_WRONLY);
	char buf[1024];
	printf("Fifo\n\n");
	while(1){
		scanf("%s",buf);
		write(fd,buf,1024);
	}
}