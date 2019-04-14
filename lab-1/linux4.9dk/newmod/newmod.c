#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

int init_module(void){
	printk(KERN_INFO "Hello World from Daniel Kramer (10426217)\n");
	return 0; /* load success */
}
void cleanup_module(void){
	printk(KERN_INFO "Current proccess is pid %i: \"%s\"\n", current->pid, current->comm);
	return;
}
