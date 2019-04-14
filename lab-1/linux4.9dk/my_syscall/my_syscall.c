#include <linux/kernel.h>

asmlinkage int sys_my_syscall(int a, int b){
	int sum = a + b;
	printk(KERN_INFO "Sum: %d\n", sum);
	return sum;
}
