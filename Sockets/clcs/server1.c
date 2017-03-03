#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#define derror(msg) {printf("%s\n", msg); exit(0);}

int csfd;

int main()
{
	int i;
	char buf[256];
	memset(buf,0,256);
	while(recv(0,buf,256,0)<0);
	for(i=0;i<strlen(buf);i++)
		buf[i]=toupper(buf[i]);
	send(0,buf,256,0);
	return 0;
}