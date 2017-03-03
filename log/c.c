#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<poll.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

void main()
{
        char buf[2][20];
        int ffd;
        ffd=open("sff1",O_WRONLY);
        printf("Enter service number:");
        scanf("%s",buf[0]);
        printf("Enter fifo name:");
        scanf("%s",buf[1]);
        write(ffd,buf,sizeof(buf));
        mkfifo(buf[1],0666);
        int cff;
        cff=open(buf[1],O_RDWR);
        char msg[80];
        while(1)
        {
                read(cff,msg,sizeof(msg));
                printf("%s\n",msg);
        }
}





#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<poll.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

void main()
{
        char buf[2][20];
        int ffd;
        ffd=open("sff2",O_WRONLY);
        printf("Enter service number:");
        scanf("%s",buf[0]);
        printf("Enter fifo name:");
        scanf("%s",buf[1]);
        write(ffd,buf,sizeof(buf));
        mkfifo(buf[1],0666);
        int cff;
        cff=open(buf[1],O_RDWR);
        char msg[80];
        while(1)
        {
                read(cff,msg,sizeof(msg));
                printf("%s\n",msg);
        }
}









#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<poll.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

void main()
{
        char buf[2][20];
        int ffd;
        ffd=open("sffd1",O_WRONLY);
        printf("Enter service number:");
        scanf("%s",buf[0]);
        printf("Enter fifo name:");
        scanf("%s",buf[1]);
        write(ffd,buf,sizeof(buf));
        mkfifo(buf[1],0666);
        int cff;
        cff=open(buf[1],O_RDWR);
        char msg[80];
        while(1)
        {
                read(cff,msg,sizeof(msg));
                printf("%s\n",msg);
        }
}
