#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>


typdef struct virt_addr{
	uint64_t allocated :1;
	uint64_t phy_addr;

}virt_addr;
typedef struct addr{
	uint32_t first :10;
	uint32_t second :10;
	uint32_t zeroes:12;
}addr;
typedef struct page_two{
	virt_addr addr[256];
}page_two;


typedef struct page_one{
	pthread_t pid;
	uint32_t valid:1;
	page_two *second_table;
}page_one;
page_one process_array[4];

uint64_t cse320_virt_to_phys(){

	uint64_t total_byte=va.first;
	total_byte+=va.second;
	return total_byte;
}

void cse320_malloc(virt_addr va){

	
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
	//	printThreads();
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
					pthread_create(&process_array[i].pid,NULL,thread,NULL);
					page_two newpagetwo;
					process_array[i].second_table=&newpagetwo;
					
					loop=1;	
				}
				i++;
			}
		}
		else if(strcmp(tok,"list")==0){
			int i=0;
			while(i<4){
				if (process_array[i].pid!=0){
					printf("%lu\n",process_array[i].pid);
				}
				i++;

			}
		}
		else if(strcmp(tok,"mem")==0){

		}
		else if (strcmp(tok,"allocate")==0){
			tok=strtok(NULL," ");
			char* ptr;
			int found=0;
			uint64_t newpid= strtoul(tok,&ptr,10);
			//go through and find the process
			int i=0;
			for(i;i<4;i++){
				if (process_array[i].pid==newpid){
					int found=1;
					int j=0;
					//go into second page table and allocated a space
					while(process_array[i].second_table->addr[j].valid!=0 && j<256){
							
						j++;
					}
					if(j=256){
						perror("No more unallocated addresses");
					{
					process_array[i].second_table->addr[j].valid==1;
				}
			}
			if(!found){
				perror("Process not found\n");
			}
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
					process_array[i].pid=0;

					//FLUSH THE TABLE AND SHIT LATER
				}
				i++;
			}
			if(!kill){
				perror("Error, id does not exist\n");
			}
			

		}
		else{
                        printf("Invalid command, press help for list of commands\n");
       
                }
	}
	return 0;

}
