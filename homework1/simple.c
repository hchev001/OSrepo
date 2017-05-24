
/*
 *        COP4610 - Principles of Operating Systems - Summer c 2017
 *        Prof. Jose F. Osorio
 *        Student: Hamilton Chevez - 3350827
 *
 *        Assignment #: 1
 *        Specs: Creating Linux Kernel Modules
 *        Due Date: 05/24/2017 by 11:55pm
 *
 *        Module Name: simple.c
 *
 *
 *        I certify that this program code has been written by me and no part of it has been taken from any sources.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

// function prototypes
void add_element(int month, int day, int year);
void delete_list(void);

// Declared a linked list
static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	// create pointer 
	struct birthday *ptr;

	add_element(10, 22, 1963);
	add_element(8, 13, 1995);
	add_element(9, 2, 1998);
	add_element(8, 12, 1963);
	
	list_for_each_entry(ptr, &birthday_list, list) {
		printk(KERN_INFO "Birthday: Month %d Day %d Year %d\n",
				ptr->month,
				ptr->day,
				ptr->year);
	}

	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void){
	printk(KERN_INFO "Removing Module\n");

	//delete linked list and free its members
	delete_list();
}

/* Creates a new instance of birthday struct with given params and adds to linked list
 * 
 * @month the birthday month
 * @day the birthday day
 * @year the year of the birthday
 *
 **/

void add_element(int month, int day, int year){
	struct birthday *person;

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->month = month;
	person->day = day;
	person->year = year;
	INIT_LIST_HEAD(&person->list);

	list_add_tail(&person->list, &birthday_list);
}

/* Deletes linked lists and frees its elements */
void delete_list(void) {
	struct birthday *currnt, *next;
	list_for_each_entry_safe(currnt, next, &birthday_list, list)
	{
		printk(KERN_INFO "Removing %d %d %d\n",
				currnt->month,
				currnt->day,
				currnt->year);
		list_del(&currnt->list);
		kfree(currnt);
	}
}
/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("creating linux kernel modules");
MODULE_AUTHOR("Hamilton Chevez");
