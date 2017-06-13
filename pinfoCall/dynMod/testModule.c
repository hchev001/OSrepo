#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <unistd.h>


int simple_init(void)
{	
	printk(KERN_INFO "Loading Module\n");

	struct task_struct *thisTask = get_current();

	printk(KERN_INFO "This process currend it is %d", thisTask->pid);
	printk(KERN_INFO "This process parent process id is %d", thisTask->parent->pid);


	printk(KERN_INFO "This process state is %lu", thisTask->state);

	// let's creathe some processes =)
	// pid_t parent;
	
	// parent = getpid();
	/* This didn't work, I forgot I was tryint to create a process from within the kernel
	 * using userspace calls
	child0 = fork();

	if(getpid() == parent)
		child1 = fork();
	
	else if (getpid() == child0)
			grandChild0 = fork();
	
	if (getpid() == child1)
		sleep(10);
	
	if (getpid() == grandChild)
		sleep(10);
	else if (getpid() == child0)
		sleep(4);
	else {
		printf("\nParent ID: %d Child0 ID: %d Child1 ID: %d", parent, child0, child1);
	*/


}

void simple_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
}

module_init( simple_init );
module_exit( simple_exit );
