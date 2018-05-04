#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int fd;
void * mem;
char buf[255];
char buf2[255];
char* tok;
uint64_t addr;
char* ptr;
int main(){
	mem=malloc(1024);
	char *fifo="fifo";
	mkfifo(fifo,0666);
	
	while(1){
		fd=open(fifo,O_RDONLY);
		read(fd,buf,sizeof(buf));
		tok=strtok(buf," ");
		close(fd);
		if (strcmp(tok,"read")==0){
			//get address
			tok=strtok(NULL," ");
			addr=strtoul(tok,&ptr,10);
			if(addr%4!=0){
				fd=open(fifo,O_WRONLY);
				//pass errror message?							
			}
			else if (addr<0 || addr>1023){
				//fd=open(fifo,O_WRONLY);

			}
			else{
			//	fd=open(fifo,O_WRONLY);

			}
		}

		else if (strcmp(tok,"write")==0){
			tok=strtok(NULL," ");
			addr=strtoul(tok,&ptr,10);
			if(addr%4!=0){
			//	fd=open(fifo,O_WRONLY);
						
			}
			else if (addr<0 || addr>1023){
			//	fd=open(fifo,O_WRONLY);

			}
			else{

			}

		}
		
		
	}
	free(mem);
	return 0;
}
