#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
	int i;
	char rbuffer[256],temp[256];
	
	fgets(rbuffer,255,stdin);
	for(i=0;i<strlen(rbuffer);i++)
		{
			if(rbuffer[i]>=97)
				temp[i] = rbuffer[i]-32;
			else
				temp[i]=rbuffer[i];
			//printf("%c",temp[i]);
		}
		temp[i]='\0';
		printf("%s\n",temp);
	
}