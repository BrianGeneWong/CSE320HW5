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
typedef struct cache_block{
	int phy_addr;
	int value;
}cache_block;

typedef struct page_one{
	pthread_t pid;
	page_two *second_table;
}page_one;
page_one process_array[4];
cache_block cache[4];
//return -1 if not valid
int checkVaInputValid(char* str){
	if (strlen(str)!=32)
		return -1;

	int i=0;
	for (i;i<32;i++){
//		printf("*char is %c",*str);
		if(*str!='0' && *str!='1')
			return -1;
	
	}
	return 0;

} 
int cse320_virt_to_phys(char* str){
	//get the middle 10 bits
	if (checkVaInputValid(str)==-1){
		return -1;
	}
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
	//grab the phy addr
	int first_index =convert/256;
	int second_index= convert%256;
	if(process_array[first_index].second_table->addr[second_index].valid==0)
		return -1;
	return (process_array[first_index].second_table->addr[second_index].phy_addr);
}

//args: process index, 2nd table index
void cse320_malloc(int i, int j){
	process_array[i].second_table->addr[j].valid=1;
	int index =((i*256)+(j*4));
	process_array[i].second_table->addr[j].phy_addr=index;
//	printf("i= %d, j=%d allocated address %d\n",i,j,index);
	
}
void *thread(void *vargp){
//	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	while(1){

	}
	return NULL;

}
void kill_process(int i){

	int j=0;
	for(j;j<256;j++){
		process_array[i].second_table->addr[j].valid=0;
		process_array[i].second_table->addr[j].phy_addr=-1;	
	}
	//go into cache and clear anything related to this process
	int lowbound=i*256;
	int highbound=lowbound+256;
	int l=0;
	for (l;l<4;l++){
		if(lowbound<=cache[l].phy_addr && cache[l].phy_addr<highbound){
			cache[l].phy_addr=-1;	

		}
	}
	process_array[i].pid=0;

}
void printThreads(){
	int i=0;
	for(i;i<4;i++){
		printf("PROCESS INDEX %d\n",i);
		printf("pid= %lu\n",process_array[i].pid);
		printf("\n");
	}
	
}
void printvalids(int i){
	int j=0;
	for (j;j<256;j++){
		if(process_array[i].second_table->addr[j].valid==1)
			printf("%d %d  phy_addr: %d\n",i,j,process_array[i].second_table->addr[j].phy_addr);

	}

}
//takes process array index
int getCacheIndex(int i){
	//see if theres an empty space, if there is, return that first
	int j =0;
	for(j;j<4;j++){
		if(cache[j].phy_addr==-1 || cache[j].phy_addr==i)
			return j;

	}	
	if (0<=i && i<256){
		return 0;
	}
	else if(256<=i && i<512){
		return 1;
	}
	else if(512<=i && i<768){
		return 2;
	}
	else if(768<=i && i<1024){
		return 3;
	}
	else
		return -1;
}
int  checkCache(int pa){
	int i=0;
	for(i;i<4;i++){
		if(cache[i].phy_addr==pa){
			printf("cache hit\n");
			return i;
		}
	}
	printf("cache miss\n");
	return -1;
}
void writeToCache(int pa,int v ){
	int  cindex=getCacheIndex(pa);
//	printf("pa: %d, cache index: %d, value: %d\n",pa,cindex,v);
	if (cindex!=-1){
		if(cache[cindex].phy_addr!=-1 && cache[cindex].phy_addr!=pa){
			printf("eviction\n");
		}
	}
	cache[cindex].phy_addr=pa;
	cache[cindex].value=v;
}
//pthread_t *thread_array[4]={NULL,NULL,NULL,NULL};
//pthread_t thread_array[4];
int main(){
	char* command= malloc(255);
	char* fifo="fifo";
	int fd;
	mkfifo(fifo,0666); 
	//initiailize my cache
	int cache_index=0;
	for (cache_index;cache_index<4;cache_index++){
		cache[cache_index].phy_addr=-1;
	}
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
			int i=0;
			for (i;i<4;i++){
				if(process_array[i].pid!=0)
					kill_process(i);

			}
			free(command);	
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
				//	printf("create process %lu \n",process_array[i].pid);
				//	printf("newpagetwp valid: %d addr: %d \n", newpagetwo.addr[0].valid,newpagetwo.addr[0].phy_addr);
			
					//go through and initialize everything
					int j=0;
					for (j;j<256;j++){

						process_array[i].second_table->addr[j].valid=0;
						process_array[i].second_table->addr[j].phy_addr=-1;

					}
				}
				i++;
			}
			if(loop==0)
				printf("Already have 4 processes\n");
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
				int found =0;
				for(i;i<4;i++){
					if (process_array[i].pid==newpid){
						found=1;
//						printvalids(i);
						int j=0;
						for(j;j<256;j++){
							if(process_array[i].second_table->addr[j].valid==1){
								//first 10 digits always 1 for us
								printf("0000000000");
								int middle=0;
								middle+=(i*256);
								middle+=(j*4);

								
						//		printf("i: %d j: %d middle: %d\n",i,j,middle);
								if(middle==0){
									printf("0000000000");
								}
								else{
									int power=512;
									while(power>0){
										if(middle/power==0)
											printf("0");
										else{
											middle=middle-power;
											printf("1");
										}
										power=power/2;
									}
								}
								
								printf("000000000000\n");
			
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
					if (process_array[i].pid==newpid && newpid!=0){
						found=1;
						int j=0;
						int allocated=0;
						//go into second page table and allocated a space
						while(allocated==0 && j<64){
							if(process_array[i].second_table->addr[j].valid==0){
								allocated=1;
								cse320_malloc(i,j);
							}
							j++;
						}
						if(allocated==0){
							printf("No More Mem for This Process\n");
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
							int pa=cse320_virt_to_phys(tok);
							//check the cache first
							int if_cached=checkCache(pa);
							if (if_cached!=-1){
								printf("%d\n",cache[if_cached].value);
							}
							else if(pa!=-1){
								int retval=cse320_virt_to_phys(tok);
								char* buf=malloc(255);
								strcpy(buf,"read,");
//								printf("pa: %d retval is: %d\n",pa,retval);
								char* retval_string=malloc(10);
								sprintf(retval_string,"%d",retval);
								strcat(buf,retval_string);	
								fd=open(fifo,O_WRONLY);
//								printf("final string is: %s\n",buf);
								write(fd,buf,255);
								close(fd);
								memset(buf,0,255);	
								fd=open(fifo,O_RDONLY);
								read(fd,buf,255);
								close(fd);
									
								int bufnum=atoi(buf);
								writeToCache(retval,bufnum);	
								
								printf("%s\n",buf);			
								free(buf);
							}
							else{

								printf("invalid virtual memory address\n");
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
					//		printf("pa=%d \n", pa);
							if (pa!=-1 ){ 
								//check if it's in the correct range for the process 			
								tok=strtok(NULL," ");
								if(tok==NULL){
									printf("print value you want to write\n");
								}
								else{
									int cindex=checkCache(pa);
									char* buf=malloc(255);
									char* pa_string=malloc(255);	
									int value = atoi(tok);
									sprintf(pa_string,"%d",pa);
									strcpy(buf,"write,");
									strcat(buf,pa_string);
									strcat(buf,",");
									strcat(buf,tok);
								//	write(fd,"write,",sizeof("write,"));
								//	write(fd,pa_string,sizeof(pa_string));
								//	write(fd,",",sizeof(","));
								//	write(fd,"tok",sizeof("tok"));
//									printf("final string: %s\n",buf);
									fd=open(fifo,O_WRONLY);
									write(fd,buf,255);
									close(fd);
									//write to cache
									free(buf);
									free(pa_string);
									writeToCache(pa,value);
								}
							}
							else{
								printf("invalid virtual address\n");
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
					//go through process and set all valid bits to 0
					kill_process(i);
				}
				i++;
			}
			if(!kill){
				perror("Error, id does not exist\n");
			}
			

		}
		else{
                        printf("Invalid command\n");
       
                }
	}
	return 0;

}
