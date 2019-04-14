#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(){
	int a = 5;
	int b = 56;
	int amma = syscall(548, a, b);
	printf("Running my_syscall with %d and %d returns %d\n", a, b, amma);
	char teststrings[] = "Optimasdsddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooasdasdwadadsadawddwadwadwadwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwddddalogy";
	int amma2 = syscall(549, teststrings);
	char teststrings2[] = "long lab we have here ooo";
	char ts2param[] = "long lab we have here ooo";
	int amma3 = syscall(549, teststrings2);
	printf("Running my_syscall2 with \"%s\" returns %i\n", ts2param, amma3);
	printf("Running my_syscall2 with \"%s\" returns %i\n", teststrings, amma2);
	return 0;
}
