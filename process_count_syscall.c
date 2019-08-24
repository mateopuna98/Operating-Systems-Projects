#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

//This system call counts how many forks a given process has
asmlinkage long sys_pcount(int pid){

	struct task_struct *task;
	int tiene = 0;

	for_each_process(task){

		if(task_ppid_nr(task) == pid){

			printk(" Un hijo es : %d\n",task->pid );
			tiene = 1;
		}
	}

	if(tiene ==0){
		printk("El proceso %d no tiene hijos",pid);
	}

	return 0;
}

