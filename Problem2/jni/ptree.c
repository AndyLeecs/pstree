/*
print the process tree in a depth-first-search order
*/
#include <ptree.h>

#define INIT_SIZE 1

struct prinfo* temp;
int count;

void dfs(int pid,int indent){
	if(pid == 0)
		return;
	struct prinfo p;
	int i;
	for(i = 0 ; i < count ; i++){
		p = temp[i];
		if(p.pid == pid){
			int j;
			for(j = 0 ; j < indent ;j++){
				printf("\t");
			}
			printf("%s,%d,%ld,%d,%d,%d,%d\n",p.comm,p.pid,p.state,p.parent_pid,p.first_child_pid,p.next_sibling_pid,p.uid);
			break;
		}
	}
	dfs(p.first_child_pid,indent+1);
	dfs(p.next_sibling_pid,indent);
}

int main(){
	count = INIT_SIZE;
	temp = (struct prinfo*)malloc(sizeof(struct prinfo)*count);	
	memset(temp,0,sizeof(struct prinfo)*count);
	
	/**allocate memory**/
	while(syscall(287,temp,&count) > count){
		free(temp);
		count = 2*count;
		temp = (struct prinfo*)malloc(sizeof(struct prinfo)*count);
		memset(temp,0,sizeof(struct prinfo)*count);
	};
	
	/**dfs**/
	dfs(temp[0].pid,0);
	
	return 0;
}	

