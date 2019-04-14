#include <linux/kernel.h>
#include <linux/string.h>

asmlinkage int sys_my_syscall2(char input[]){
	if (strlen(input)>128) return -1;
	int replacements;
	replacements = 0;
	int i;
	for(i = 0; i < strlen(input); i++){
		if(input[i] == 'o') {
			input[i] = '0';
			replacements++;
		}
	}
	return replacements;
}
