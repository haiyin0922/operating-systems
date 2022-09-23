#include <linux/sched/task.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

void dfs(struct task_struct *task) {
	struct task_struct *child;
	struct list_head *list;
	
	printk(KERN_INFO "pid: %d | pname: %s | state: %d\n", task->pid, task->comm, task->state);
	
	list_for_each(list, &task->children) {
		child = list_entry(list, struct task_struct, sibling);
		dfs(child);
	}
}

/* This function is called when the module is loaded. */
int entryPoint(void) {
	printk(KERN_INFO "Loading Module\n");	

	dfs(&init_task);
	
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
MODULE_DESCRIPTION("List tasks using DFS");
MODULE_AUTHOR("105072123");
