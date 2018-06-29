/*
a program to try on fork and execl
*/
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){

	int i = 1;
	pid_t pid;

	printf("716030210006Parent is %d\n",getpid());
	pid = fork();
	
	//error
	if(pid<0){
		fprintf(stderr,"Fork Failed");
		return 1;
	}
	
	//child process
	else if(pid == 0){
		printf("716030210006Child is %d\n",getpid());
		execl("/data/misc/ptree","ptree",NULL);
	}

	//parent process	
	else{
		wait(NULL);
	}
	
	return 0;

}

