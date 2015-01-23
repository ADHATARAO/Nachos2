#include "syscall.h"
int main()
{
	PutString("Files opened...\n");
	ForkExec("userpages0");
	PutChar('\n');
	ForkExec("userpages1");

	PutString("Exiting...\n");
Halt();
}
