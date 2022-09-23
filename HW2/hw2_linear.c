#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/* This function is called when the module is loaded. */
int entryPoint(void) {
	struct task_struct *task;

	printk(KERN_INFO "Loading Module\n");

	for_each_process(task) {
		printk(KERN_INFO "pid: %d |pname: %s |state: %d\n", task->pid, task->comm, task->state);
	}
	
	printk(KERN_INFO "---------------END---------------\n");
	return 0;
}

/* This function is called when the module is removed. */
void exitPoint(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(entryPoint);
module_exit(exitPoint);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("List tasks linearly");
MODULE_AUTHOR("105072123");
