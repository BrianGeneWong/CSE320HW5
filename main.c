#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
typedef struct addr{
	uint32_t valid :10;
	uint32_t entry :10;
	uint32_t zeroes:12;
}addr;

typedef struct page_one{
	pthread_t pid;
	uint32_t valid:1;
	uint32_t *page_two[4];
}page_one;
typedef struct page_two{
	uint32_t addr[64];
}page_two;

page_one process_array[4];


void cse320_malloc(){

}
void cse320_virt_to_phys(){

}
void *thread(void *vargp){
	while(1){

	}
	return NULL;

}
void printThreads(){
	int i=0;
	for(i;i<4;i++){
		printf("PROCESS INDEX %d\n",i);
		printf("pid= %lu\n",process_array[i].pid);
		printf("valid = %d\n",process_array[i].valid);
		printf("\n");
	}
	
}
//pthread_t *thread_array[4]={NULL,NULL,NULL,NULL};
//pthread_t thread_array[4];
int main(){
	char* command= malloc(255);
	while(1){
		printThreads();
		printf("Input Command: ");
                fgets(command,255,stdin);
                char* tok;
                char *replace;
                if((replace=strchr(command,'\n'))!=NULL){
                        *replace=0;
                }
		tok=strtok(command," ");
                if(strcmp(tok,"exit")==0){
			int i=0;
			exit(0);
                }
		else if(strcmp(tok, "create")==0){
			int i=0;
			int loop=0;
			while(!loop &&  i<4){
				if(process_array[i].pid==0){
					page_one newpage;
					pthread_create(&newpage.pid,NULL,thread,NULL);
					process_array[i]=newpage;
					loop=1;	
				}
				i++;
			}
		}
		else if(strcmp(tok,"list")==0){


		}
		else if(strcmp(tok,"mem")==0){

		}
		else if (strcmp(tok,"allocate")==0){
		}
		else if (strcmp(tok,"read")==0){
		}
		else if (strcmp(tok,"write")){
		}
		
		else if(strcmp(tok,"kill")==0){	
			tok=strtok(NULL," ");
			int i=0;
			while(i<4){
				if(process_array[i].pid!=0)
					pthread_cancel(process_array[i].pid);
				i++;
			}
			

		}
		else{
                        printf("Invalid command, press help for list of commands\n");
       
                }
	}
	return 0;

}
