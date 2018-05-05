#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <fcntl.h>
typedef struct virt_addr{
	uint32_t valid :1;
	int phy_addr;

}virt_addr;
/*
typedef struct addr{
	uint32_t first :10;
	uint32_t second :10;
	uint32_t zeroes:12;
}addr;*/
typedef struct page_two{
	virt_addr addr[256];
}page_two;


typedef struct page_one{
	pthread_t pid;
	page_two *second_table;
}page_one;
page_one process_array[4];

void index_to_va(int i,int j){
	//first print 10 zeroes
	printf("0000000000");
	

}
int cse320_virt_to_phys(char* str){
	//get the middle 10 bits
	int convert=0;
	str+=10;
	int power=512;
	int i=0;
	for(i;i<10;i++){
		if (*str=='0'){
			//dont add anything for a 0
		}
		else if (*str=='1'){
			convert+=power;
		}
		power=power/2;
		str++;
	}
	return convert;
}

//args: process index, 2nd table index
void cse320_malloc(int i, int j){
	process_array[i].second_table->addr[j].valid=1;
	int index =((i*256)+(j*4));
	process_array[i].second_table->addr[j].phy_addr=index;
	printf("allocated address %d\n",index);
	
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
		printf("\n");
	}
	
}
//takes process array index
void clearMem(int i){

	
}
//pthread_t *thread_array[4]={NULL,NULL,NULL,NULL};
//pthread_t thread_array[4];
int main(){
	char* command= malloc(255);
	char* fifo="fifo";
	int fd;
	mkfifo(fifo,0666); 

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
		if(tok==NULL){
			printf("Please input a command\n");
		}
                else if(strcmp(tok,"exit")==0){
			//clean physical mem
				
			exit(0);
                }
		else if(strcmp(tok, "create")==0){
			int i=0;
			int loop=0;
			while(loop==0 &&  i<4){
				if(process_array[i].pid==0){
					pthread_create(&process_array[i].pid,NULL,thread,NULL);
					page_two newpagetwo;
				//	printf("newpagetwp valid: %d addr: %d \n", newpagetwo.addr[0].valid,newpagetwo.addr[0].phy_addr);
					process_array[i].second_table=&newpagetwo;
					loop=1; 
					newpagetwo.addr[0].valid=0;
						
				//	printf("create process %lu \n",process_array[i].pid);
				//	printf("newpagetwp valid: %d addr: %d \n", newpagetwo.addr[0].valid,newpagetwo.addr[0].phy_addr);
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
			tok=strtok(NULL," ");
			char* ptr;
			int found=0;
			//go through and find the process
			int i=0;
			if (tok!=NULL){	
				uint64_t newpid= strtoul(tok,&ptr,10);
				for(i;i<4;i++){
					if (process_array[i].pid==newpid){
						int j=0;
						for(j;j<256;j++){
							if(process_array[i].second_table->addr[j].valid==1){
								//cse320_virt_to_phys()
							}
						}			
					}
				}
			}
			else{

				printf("please input process X\n");
			}
		}
		else if (strcmp(tok,"allocate")==0){
			tok=strtok(NULL," ");
			char* ptr;
			int found=0;
			//go through and find the process
			int i=0;
			if (tok!=NULL){
				uint64_t newpid= strtoul(tok,&ptr,10);
				for(i;i<4;i++){
					if (process_array[i].pid==newpid){
						found=1;
						int j=0;
						int allocated=0;
						//go into second page table and allocated a space
						while(allocated==0 && j<256){
							printf("j is current %d\n",j);
							printf("valid bit is %d",process_array[i].second_table->addr[j].valid);
							if(process_array[i].second_table->addr[j].valid==0){
								allocated=1;
								cse320_malloc(i,j);
							}
							j++;
						}
					}
				}
				if(found==0){
					printf("Process not found\n");
				}
			}
			else{

				printf("please input process X\n");
			}
		}
		else if (strcmp(tok,"read")==0){
			tok=strtok(NULL," ");
			char* ptr;
			if (tok!=NULL){
				uint64_t newpid= strtoul(tok,&ptr,10);
				int i=0;
				int found=0;
				for(i;i<4;i++){
					if (process_array[i].pid==newpid){
						found=1;
						int j=0;
						tok=strtok(NULL," ");
						if(tok==NULL)
							printf("Virtual address Y needed\n");
						else{
							uint64_t va= strtoul(tok,&ptr,10);
							
						}	
					}
				}
				if(found==0){
					printf("Process not found\n");
				}
			}
			else{

				printf("please input process X and address Y\n");
			}
		}
		else if (strcmp(tok,"write")==0){
			tok=strtok(NULL," ");
			char* ptr;
			if (tok!=NULL){
				//turn the binary string into an int 
				int i=0;
				int found=0;	
				uint64_t newpid= strtoul(tok,&ptr,10);
				for(i;i<4;i++){
					if (process_array[i].pid==newpid){
						found=1;
						int j=0;
						tok=strtok(NULL," ");
						if(tok==NULL)
							printf("Virtual address Y needed\n");
						else{
							int pa=cse320_virt_to_phys(tok);
							//check if it's in the correct range for the process 			
							tok=strtok(NULL," ");
							if(tok==NULL){
								printf("print value you want to write\n");
							}
							else{
								char* buf=malloc(255);
								char* pa_string=malloc(255);
								int value = atoi(tok);
								sprintf(pa_string,"%d",pa);
								strcat(buf,"write,");
								strcat(buf,pa_string);
								strcat(buf,",");
								strcat(buf,tok);
							//	write(fd,"write,",sizeof("write,"));
							//	write(fd,pa_string,sizeof(pa_string));
							//	write(fd,",",sizeof(","));
							//	write(fd,"tok",sizeof("tok"));
								printf("final string: %s\n",buf);
								fd=open(fifo,O_WRONLY);
								write(fd,buf,sizeof(buf));
								close(fd);	
								free(buf);
								free(pa_string);
							}
							

						}	
					}
				}
				if(found==0){
					printf("Process not found\n");
				}
			}
			else{
				printf("please input process X and address Y\n");
			}
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
