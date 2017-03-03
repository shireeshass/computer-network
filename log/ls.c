#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<poll.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

void main()
{
        //printf("s1\n");
        char ffname[80];
        read(STDIN_FILENO,ffname,sizeof(ffname));
        //printf("%s",ffname);
        int ffd;
        ffd=open(ffname,O_RDWR);
        char msg1[80],msg2[80];
        strcpy(msg1,"Service 1 is serving....");
        strcpy(msg2,"Finished.");
        write(ffd,msg1,sizeof(msg1));
        sleep(2);
        write(ffd,msg2,sizeof(msg2));
        close(ffd);
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
        //printf("s1\n");
        char ffname[80];
        read(STDIN_FILENO,ffname,sizeof(ffname));
        //printf("%s",ffname);
        int ffd;
        ffd=open(ffname,O_RDWR);
        char msg1[80],msg2[80];
        strcpy(msg1,"Service 2 is serving....");
        strcpy(msg2,"Finished.");
        write(ffd,msg1,sizeof(msg1));
        sleep(2);
        write(ffd,msg2,sizeof(msg2));
        close(ffd);
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
        //printf("s1\n");
        char ffname[80];
        read(STDIN_FILENO,ffname,sizeof(ffname));
        //printf("%s",ffname);
        int ffd;
        ffd=open(ffname,O_RDWR);
        char msg1[80],msg2[80];
        strcpy(msg1,"Service 3 is serving....");
        strcpy(msg2,"Finished.");
        write(ffd,msg1,sizeof(msg1));
        sleep(2);
        write(ffd,msg2,sizeof(msg2));
        close(ffd);
}
