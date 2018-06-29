#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>

#include<asm/uaccess.h>

struct prinfo{
	pid_t parent_pid;	/* process id of parent */
	pid_t pid;			/* process id */
	pid_t first_child_pid;	/* pid of youngest child */
	pid_t next_sibling_pid; /* pid of older sibling */
	long state;			/* current state */
	long uid;		/* user id of process owner*/
	char comm[64];		/* name of program executed*/
};
