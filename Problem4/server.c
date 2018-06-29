/*
server side program of Caesar Encryption
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define N 256//the buffer size

#define MAX_CLIENTS 2//the maximum clients to be concurrently served
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//protect the counter 
int counter=0;//count the current number of clients
int come();//increase counter, return 1 if can be served, return 0 if can not
void leave();//decrease counter

void*serve(void* newsockfd);
void cipher(char* s);

int main(int argc, char* argv[]){
	int sockfd,newsockfd,portno,clilen;
	
	struct sockaddr_in serv_addr,cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("ERROR opening socket\n");
		exit(1);
	}
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	portno = 2050;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
		printf("Error on binding\n");
		exit(1);
	}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	printf("Server initiating...\n");
	
	//multi-thread	
	while(1){				
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, 
                 &clilen);               
		pthread_t thread;	
		pthread_create(&thread,NULL,serve,&newsockfd);			
	}
	return(0);
}
void *serve(void *sockfd){
	int newsockfd = (int)(*((int*)sockfd));
	int n = 0;
	char buffer[N];
	while(1){
	//encryption		
		memset(buffer,'\0',N);
		n = read(newsockfd,buffer,N);
		if(n<0){
			printf("Server thread closing\n");
			close(newsockfd);
			leave();
			pthread_exit(0);
		}
	//exit
		if(buffer[0] == ':' && buffer[1] == 'q' && buffer[2] == '\n'){
			printf("Server thread closing\n");
			close(newsockfd);
			leave();
			pthread_exit(0);
		}
		printf("Receiving message: %s",buffer);
	//serve the threads that do not have to wait
		if(come()){
		cipher(buffer);
		n = write(newsockfd,buffer,N);
		}else{
	//inform the threads that have to wait
		n = write(newsockfd,"Sorry,please wait\n",19);
		}
		memset(buffer,'\0',N);
		if(n<0){
			printf("Server thread closing\n");
			close(newsockfd);
			leave();
			pthread_exit(0);
		}
	}
}

int come(){
	pthread_mutex_lock( &mutex);	
	if(counter>=MAX_CLIENTS){	
	pthread_mutex_unlock( &mutex);	
	return 0;
	}
	else{
	counter++;
	pthread_mutex_unlock(&mutex);
	return 1;
	}	
}
void leave(){
	pthread_mutex_lock( &mutex);
	counter--;
	pthread_mutex_unlock( &mutex);
}
void cipher(char* s){
	for(int i = 0 ; s[i] != '\n' && s[i] != '\0' ; i++){
		if (s[i] <= 'Z'&&s[i] >= 'A') {
			s[i] = 'A' + ((s[i] - 'A' + 3) % 26);
		}
		else if (s[i] <= 'z'&&s[i] >= 'a') {
			s[i] = 'a' + ((s[i] - 'a' + 3) % 26);
		}
	}
}
