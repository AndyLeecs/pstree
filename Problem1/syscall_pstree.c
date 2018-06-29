/*
a system call to get the process tree
*/
#include "syscall_pstree.h"

MODULE_LICENSE("Dual BSD/GPL");

#define __NR_pstreecall 287

static int (*oldcall)(void);

static int ptree(struct prinfo*buf, int *nr){
	/**get the size of available space from user**/
	int cnt=0,knr;	
	if(copy_from_user(&knr,nr,sizeof(int)))
		return -1;
//	printk(KERN_INFO"copy from user!\n");
	{		
	/**grab the tasklist_lock**/	
	read_lock(&tasklist_lock);
//	printk(KERN_INFO"read lock!\n");

	/**construct prinfo by task_struct of each process**/		
	{
	struct task_struct* p;
//	printk(KERN_INFO"start iteration!\n");

	for_each_process(p){
//	printk(KERN_INFO"in iteration!\n");
	if(cnt<knr){
	struct prinfo pr;
//	printk(KERN_INFO"after having pr!\n");
	pr.pid = p->pid;
	pr.parent_pid = p->parent->pid;
	pr.first_child_pid = 0;
	pr.next_sibling_pid = 0;

//	printk(KERN_INFO"before checking children!\n");
	/**check if the process has children**/
	if(!list_empty(&p->children))
		pr.first_child_pid = list_entry(p->children.next,struct task_struct,sibling)->pid;
	
//	printk(KERN_INFO"before checking sibling!\n");
	/**check if the process has next sibling**/
	if(!(p->sibling.next == p->parent->children.next))
		pr.next_sibling_pid = list_entry(p->sibling.next,struct task_struct,sibling)->pid;
	
//	printk(KERN_INFO"before checking end!\n");	
	/**check if the doubly linked list has reached its end**/
	if(pr.next_sibling_pid == 1)
		pr.next_sibling_pid = 0;

//	printk(KERN_INFO"before copying state!\n");
	pr.state = p->state;	

//	printk(KERN_INFO"before copying uid!\n");		
	pr.uid = p->cred->uid;	

//	printk(KERN_INFO"before copying comm!\n");	
	strcpy(pr.comm,p->comm);
	
	printk(KERN_INFO"%s,%d,%ld,%d,%d,%d,%d\n",pr.comm,pr.pid,pr.state,pr.parent_pid,pr.first_child_pid,pr.next_sibling_pid,pr.uid);
//	printk(KERN_INFO"before copying to user!\n");	
	/**copy process to user**/
	
	if(copy_to_user(buf+cnt,&pr,sizeof(pr))){
		printk(KERN_INFO"copy to user error!\n");
		return -1;
	}
//	printk(KERN_INFO"copy to user %d!\n",cnt);
	}
	cnt++;
	printk(KERN_INFO"count is %d!\n",cnt);
}	

	/**unlock tasklist_lock**/		
	read_unlock(&tasklist_lock);
	printk(KERN_INFO"unlock!\n");
}	
}
	printk(KERN_INFO"copy size to user!\n");
	/**copy size to user**/
	if(cnt<knr)
	if(copy_to_user(nr,&cnt,sizeof(int)))
		return -1;
	
	printk(KERN_INFO"before return!\n");
	return cnt;
}

static int addsyscall_init(void){
    long*syscall=(long*)0xc000d8c4;
    oldcall=(int(*)(void))(syscall[__NR_pstreecall]);
    syscall[__NR_pstreecall]=(unsigned long)ptree;
    printk(KERN_INFO"module load!\n");
    return 0;
}

static void addsyscall_exit(void){
    long*syscall=(long*)0xc000d8c4;
    syscall[__NR_pstreecall]=(unsigned long)oldcall;
    printk(KERN_INFO"module exit!\n");
}
module_init(addsyscall_init);
module_exit(addsyscall_exit);
