#include "syscall.h"
int main()
{
	PutString("Files opened...\n");
	ForkExec("userpages1");
	PutChar('\n');
	ForkExec("userpages2");
	PutChar('\n');
	ForkExec("userpages1");
	PutChar('\n');
	ForkExec("userpages2");
	PutChar('\n');
	ForkExec("userpages1");
	PutChar('\n');
	ForkExec("userpages2");
	PutChar('\n');
	ForkExec("userpages1");
	PutChar('\n');
	ForkExec("userpages2");
	PutChar('\n');
	ForkExec("userpages1");
	PutChar('\n');
	ForkExec("userpages2");
	PutChar('\n');
	ForkExec("userpages1");
	PutChar('\n');
	ForkExec("userpages2");
	PutChar('\n');
	PutString("\nExiting...\n");
Halt();
}
