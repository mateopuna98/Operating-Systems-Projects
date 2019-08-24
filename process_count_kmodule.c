k#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mateo Puña Mercado");
MODULE_DESCRIPTION("Un módulo que cuenta la cantidad de hijos de un proceso");
MODULE_VERSION("0.1");

static int pid = 1;

module_param(pid, int, S_IRUGO);
//MODULE_PARAM_DESC("El proceso del cual se obtendran los hijos");

static int __init pcount_lkm_init(void){

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

static void __exit pcount_lkm_exit(void){

	printk( KERN_INFO "Adios!\n");
}

module_init(pcount_lkm_init);
module_exit(pcount_lkm_exit);