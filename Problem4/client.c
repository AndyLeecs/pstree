/*
client side program of Caesar Encryption
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define N 256//the buffer size
int main(int argc, char *argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[N];
	//the port number of server
	portno = 2050;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		printf("ERROR opening socket");
		exit(1);
	}
	server = gethostbyname("127.0.0.1");
	if(server == NULL){
		printf("ERROR, no such host");
		exit(0);
	}
	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr,
	server->h_length);
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd, (struct sockaddr *)&serv_addr, 
	sizeof(serv_addr)) < 0){
		printf("ERROR connecting");
		exit(1);
	}
	printf("welcome to the caesar encryption client\n");
	printf("the longer than %i characters part of plaintext will not be ciphered\n",N-1);
	printf("input :q to end the service\n");
	printf("Please enter the plaintext\n");
	//sending message to server		
	while(1){

		memset(buffer,'\0',N);
		fgets(buffer,N,stdin);
		fflush(stdin);
		n = write(sockfd,buffer,strlen(buffer));
		//exit
		if(buffer[0] == ':' && buffer[1] == 'q' && buffer[2] == '\n'){
		printf("client closing...\n");
		close(sockfd);
		return(0);
		}
		if(n<0){
		perror("ERROR writing to socket");
		exit(0);
		}
		//not exit
		memset(buffer,'\0',N);
		n = read(sockfd,buffer,N);
		if(n < 0){
		perror("ERROR reading from socket");
		exit(0);
		}
		printf("From Server: %s",buffer);
	}
		close(sockfd);
		return 0;
}
