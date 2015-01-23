#include "syscall.h"


void print(void * arg)
{
	char *c= (char*) arg;
	int i=0;
	PutChar('\n');
	for (i=0;i<100;i++)
	{
		PutChar(*c);
		//currentThread->Yield();
	}

	UserThreadExit();
}

int main ()
{

	char a='Z';
	char b='X';

	PutString("start of main function in Userpages0\n");
	int id1= UserThreadCreate(print,(void*)&a );
	PutString("middle of main function in Userpages0\n");
	int id2= UserThreadCreate(print,(void*)&b );

	UserThreadJoin(id1);
	UserThreadJoin(id2);
	PutString("\nend of main function in Userpages0\n");
	return 0;
}
