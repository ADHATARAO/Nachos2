#include "syscall.h"
#define THIS "aa"
#define THAT "bb"
const int N = 20; // Choose it large enough!

void abc(char *s){

	char *p;
	for (p = s; *p != '\0'; p++)
	{
		PutChar(*p);

	}
	//PutChar('\n');


}

void f(void *s)
{
	int i;
	for (i = 0; i < N; i++)
	{
		abc((char *)s);
		//PutChar('\n');
	}
//	UserThreadExit();
	//PutChar('\n');
}


int main(){
	PutString("\nmain of user Pages 2 begins: \n");
	//PutChar('\n');
	int id= UserThreadCreate(f, (void *) THIS);
	UserThreadJoin(id);
	f((void*) THAT);
	
	//PutChar('\n');
	return 0 ;
}
