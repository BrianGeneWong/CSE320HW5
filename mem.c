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

char* oob_error= "error,address out of range";
char* align_error="error,address is not aligned";
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
				write(fd,align_error,sizeof(align_error));							
			}
			else if (addr<0 || addr>1023){
				//fd=open(fifo,O_WRONLY);
				write(fd,oob_error,sizeof(oob_error));
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
				write(fd,align_error,sizeof(align_error));							
						
			}
			else if (addr<0 || addr>1023){
			//	fd=open(fifo,O_WRONLY);
				write(fd,oob_error,sizeof(oob_error));

			}
			else{

			}

		}
		
		
	}
	free(mem);
	return 0;
}
