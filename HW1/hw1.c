#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

struct data {
	int id;
    int year;
    int month;
	int day;
	struct list_head list;
};

struct data *setting (int id, int year, int month, int day){
	struct data *student = kmalloc(sizeof(struct data), GFP_KERNEL);
	student->id = id;
	student->year = year;
	student->month = month;
	student->day = day;

	return student;
}

static LIST_HEAD(student_list);

/* This function is called when the module is loaded. */
int simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	
	struct data *student = setting(105072111, 1997, 9, 1);
	list_add_tail(&student->list, &student_list);
	student = setting(105072222, 1997, 10, 2);
	list_add_tail(&student->list, &student_list);
	student = setting(105072333, 1997, 11, 3);
    list_add_tail(&student->list, &student_list);
	student = setting(105072444, 1997, 12, 4);
    list_add_tail(&student->list, &student_list);
	student = setting(105072555, 1998, 1, 5);
    list_add_tail(&student->list, &student_list);
	
	struct data *ptr;
	list_for_each_entry(ptr, &student_list, list) {
		printk(KERN_INFO "%d, %d-%d-%d.\n", ptr->id, ptr->day, ptr->month, ptr->year);
	}

	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void)
{
	struct data *ptr, *next;
	
	list_for_each_entry_safe(ptr, next, &student_list, list){
		list_del(&(ptr->list));
		kfree(ptr);
	}

	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
