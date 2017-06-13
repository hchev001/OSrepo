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
## Test Runner: testpinfo.c
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################
**/

#include "/usr/src/linux/include/linux/pinfo.h"
#include <stdio.h>
#include <unistd.h>
//#include <pthread.h>
#include <linux/sched.h>
#include <linux/types.h>

void * sleeping();

int main()
{
    struct pinfo p_info;
    pid_t parent;
    pid_t child0;
    pid_t child1;
    pid_t grand_child;
    //pthread_t threads[10];
    //int i, j;

    parent = getpid();

    child0 = fork();

    //create child processes
    if (getpid() == parent)
    {
      child1 = fork();

      /*
       * Couldn't create threads due to some conflicts I couldn't resolve
       * In file included from /usr/include/pthread.h:25,
                 from testpinfo.c:4:
	/usr/include/time.h:105: error: conflicting types for ‘timer_t’
	/usr/include/linux/types.h:22: error: previous declaration of ‘timer_t’ was here
 
      for ( i = 0; i < 10; i++){
	j = pthread_create(&threads[i], NULL, sleeping, NULL);
      	if(j != 0){
		printf("Error creating thread, return code of %d\n", j);
		exit(j);
	}

      }*/
      
    }
    else if (getpid() == child0)
    {
      grand_child = fork();
    }

    if (getpid() == child1)
    {
      sleep(10);
    }
    else if (getpid() == grand_child)
    {
      sleep(10);
    }
    else if (getpid() == child0)
    {
      sleep(12);
    }
    else
    {
      printf("==========================================\n");
      printf("Parent ID: %d\n", parent);
      printf("Child1 ID: %d\n", child0);
      printf("Child2 ID: %d\n", child0);
    }


    //System call in 64-bit mode
    int success = syscall(134, &p_info);


    //PRINT RESULTS
    printf("==========================================\n");
    printf("Syscall return value: %d (success if 0)\n", success);
    printf("This process PID:  %d\n", p_info.pid);
    printf("Current State of Process:  %lu\n", p_info.state);
    printf("Process NICE value:  %lu\n", p_info.nice);
    printf("Parent PID:  %d\n", p_info.parent_pid);
    printf("Number of Children:  %d\n", p_info.nr_children);
    printf("Number of Light Chidlren (threads): %d\n", p_info.nr_threads);
    printf("Youngest Child PID: %d\n", p_info.youngest_child_pid);
    printf("Younger Sibling PID: %d\n", p_info.younger_sibling_pid);
    printf("Older Sibling  PID: %d\n", p_info.older_sibling_pid);
    printf("Process start time (in ms): %lu\n", p_info.start_time/1000000);
    printf("CPU time spent in user mode (in ns): %lu\n", p_info.user_time);
    printf("CPU time spent in system mode (in ns): %lu\n", p_info.sys_time);
    printf("Total user time of chidlren (in ns): %lu\n", p_info.cutime);
    printf("Total system time of children (in ns): %lu\n", p_info.cstime);
    printf("User Id of process owner: %lu\n", p_info.uid);
    printf("Name of Program being executed:  %s\n\n", p_info.comm);



    return 0;
}

void *sleeping(){
	sleep(15);
	pthread_exit(NULL);
}
