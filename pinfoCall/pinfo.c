/*
 *
##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2017
## Prof. Jose F. Osorio
## Student Name: Hamilton Chevez - 3350827
##
## Project: Customizing Linux Kernel
## Specs:   Adding Static System Call
## Due Date: 06/12/2017 by 11:55pm
## Module Name: pinfo.c
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################
**/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include "/usr/src/linux/include/linux/pinfo.h"

struct UserSystemTimes{
	long userTimeOfChildren;
	long systemTimeOfChildren;
};

// function prototypes
int getNumberOfProcOf(struct task_struct *task);
int getNumberOfThreadsOf(struct task_struct *task);
pid_t youngestChildIdOf(struct task_struct *task);
pid_t youngerSiblingOf(struct task_struct *task);
pid_t olderSiblingOf(struct task_struct *task);
unsigned long startTimeOf(struct task_struct *task);
void assignChildrensTimes(struct task_struct *task, struct UserSystemTimes *time, int childCount);

asmlinkage long sys_pinfo(struct pinfo *info){
	
	if (&info == NULL)
		return -22;

	// temporary buffer for values to fetch from task
	struct pinfo thisInfo;
	struct UserSystemTimes *times;
	times = kmalloc(sizeof(struct UserSystemTimes), GFP_KERNEL);

	struct task_struct *currentTask = get_current();

	// the current process pid
	thisInfo.pid = currentTask->pid;

	// current state of process
	thisInfo.state = currentTask->state;

	// current process NICE value
	thisInfo.nice = task_nice(currentTask);

	// current process parent's id
	thisInfo.parent_pid = currentTask->parent->pid;

	// current process user time
	thisInfo.user_time = currentTask->utime;
	
	// current process system time
	thisInfo.sys_time = currentTask->stime;

	// current process user id
	thisInfo.uid = currentTask->user->uid;

	// total number of child processes
	thisInfo.nr_children = getNumberOfProcOf(currentTask);

	// pid of youngest child
	thisInfo.youngest_child_pid = youngestChildIdOf(currentTask);

	// pid of younger sibling
	thisInfo.younger_sibling_pid = youngerSiblingOf(currentTask);

	// pid of older sibling
	thisInfo.older_sibling_pid = olderSiblingOf(currentTask);
	// current process start time
	thisInfo.start_time = startTimeOf(currentTask);
	
	// get total user and system times of children
	assignChildrensTimes(currentTask, times, thisInfo.nr_children);
	thisInfo.cutime = times->userTimeOfChildren;
	thisInfo.cstime = times->systemTimeOfChildren;
	
	// total number of thread children of this process
	thisInfo.nr_threads = getNumberOfThreadsOf(currentTask);
	
	// name of program executed
	strncpy(thisInfo.comm, currentTask->comm, 16);

	// copy the contnets of thisInfo to user space
	if (copy_to_user(info, &thisInfo, sizeof(struct pinfo))){
		return -22;
	}

	kfree(times);
	
	return 0;
}

/*
 *	Returns number of child processes of the current task.
 *
 **/
int getNumberOfProcOf(struct task_struct *task){
	
	int count = 0;
	struct task_struct *indexBuffer;

	list_for_each_entry(indexBuffer, &task->children, children)
		count++;

	return count;
}

/*
 * 	Returns the number of threads (light processes) of the current task
 **/
int getNumberOfThreadsOf(struct task_struct *task){

	int threadCount = 1;
	
	/*
	 * Causes a deadlock in my system I believe since VM becomes unresponsive
	struct task_struct *iterator;

	while_each_thread(task, iterator){
		threadCount++;
	} */
	
	return threadCount;
}

/* Returns the youngest child pid of a task_struct */
pid_t youngestChildIdOf(struct task_struct *task){
	
	struct task_struct *child = list_entry(task->children.prev, struct task_struct, children);

	if(child->pid <= 0)
	{
		return -1;
	}
	else if(child->pid == task->pid)
	{
		return -1;
	}
	else {
		return child->pid;
	}
}

/* return the pid of the current process younger sibling */
pid_t youngerSiblingOf(struct task_struct *task){


	// get younger sibling's pid
	struct task_struct *child = list_entry(task->children.prev, struct task_struct, sibling);

	if(child->pid <= 0)
		return -1;
	else if (child->pid == task->pid)
		return -1;
	else
		return child->pid;
}

/* returns the pid of the current process older sibling */
pid_t olderSiblingOf(struct task_struct *task){
	
	//get older sibling's pid
	struct task_struct *older;
	older = list_entry(task->sibling.next, struct task_struct, sibling);

	if (older->pid <= 0){

		return -1;
	}
	else if (older->pid == task->pid){
		
		return -1;
	}
	else {
		return older->pid;
	}
}

/* 
 * Returns the current process start time 
 *
 * */
unsigned long startTimeOf(struct task_struct *task){
	
	struct timespec start_time = task->start_time;
	return start_time.tv_nsec;
}

/* Assigns the user and system times of the child processes to a UserSystemTimes struct previously declared */
void assignChildrensTimes(struct task_struct *task, struct UserSystemTimes *time, int childCount){
	
	//UserSystemTimes time;
	time->userTimeOfChildren = 0;
	time->systemTimeOfChildren = 0;

	if (childCount == 0)
		return;
	else {
		struct task_struct *child;
		struct list_head *pointer;

		list_for_each(pointer, &task->children){

			child = list_entry(pointer, struct task_struct, children);
			time->userTimeOfChildren += child->utime;
			time->systemTimeOfChildren += child->stime;
		}
	}
}
