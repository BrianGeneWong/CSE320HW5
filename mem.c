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
char buf2[255];
char* tok;
uint64_t addr;
char* ptr;

char* oob_error= "error,address out of range";
char* align_error="error,address is not aligned";
int main(){
	mem=malloc(1024);
	char* buf=malloc(255);
	char *fifo="fifo";
	mkfifo(fifo,0666);
	
	while(1){
		sleep(5);
		fd=open(fifo,O_RDONLY);
		read(fd,buf,255);
		close(fd);
		printf("%s\n",buf);
		tok=strtok(buf,",");
		if(tok==NULL){

		}
		else if (strcmp(tok,"read")==0){
			//get address
			tok=strtok(NULL,",");
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
				printf("READING STUFF\n");

			}
		}

		else if (strcmp(tok,"write")==0){
			printf("WE IN WRITE BOIS\n");
			tok=strtok(NULL,",");
			printf("tok: %s\n",tok);
			addr=strtoul(tok,&ptr,10);
			tok=strtok(NULL,",");
			int value=atoi(tok);
			if(addr%4!=0){
				printf("align error\n");
				fd=open(fifo,O_WRONLY);
				write(fd,align_error,sizeof(align_error));
				close(fd);					
						
			}
			else if (addr<0 || addr>1023){
				printf("oob error\n");
				fd=open(fifo,O_WRONLY);
				write(fd,oob_error,sizeof(oob_error));
				close(fd);
			}
			else{

				
			}

		}
		else{


		}
		
		
	}
	free(mem);
	return 0;
}
