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
typedef struct page_two{
	uint32_t addr[1024];
}page_two;


typedef struct page_one{
	pthread_t pid;
	uint32_t valid:1;
	page_two *second_table;
}page_one;
page_one process_array[4];


void cse320_malloc(){

}
void cse320_virt_to_phys(){

}
void *thread(void *vargp){
//	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
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

					page_two newpagetwo;
					process_array[i].second_table=&newpagetwo;
					
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
			tok=strtok(NULL," ");
		}
		else if (strcmp(tok,"read")==0){
		}
		else if (strcmp(tok,"write")==0){
		}
		
		else if(strcmp(tok,"kill")==0){	
			tok=strtok(NULL," ");
			char* ptr;
			uint64_t killpid=strtoul(tok,&ptr,10);
			int i=0;
			int kill=0;
			while(i<4&&!kill){
				if(process_array[i].pid==killpid){
					pthread_cancel(process_array[i].pid);
					kill=1;
					process_array[i].pid==0;

					//FLUSH THE TABLE AND SHIT LATER
				}
				i++;
			}
			if(!kill){
				printf("Error, id does not exist\n");
			}
			

		}
		else{
                        printf("Invalid command, press help for list of commands\n");
       
                }
	}
	return 0;

}
