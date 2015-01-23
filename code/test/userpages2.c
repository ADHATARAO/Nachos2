#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"
const int N = 20; // Choose it large enough!

void abc(char *s){

	char *p;
	for (p = s; *p != '\0'; p++)
	{
		PutChar(*p);

	}


}

/*void fm(void *s)
{
	int i;
	for (i = 0; i < N; i++)
	{
		abc((char *)s);
		//PutChar('\n');
	}

}*/
void f(void *s)
{
	int i;
	for (i = 0; i < N; i++)
	{
		abc((char *)s);
		//PutChar('\n');
	}
//	UserThreadExit();
}


int main(){
	PutString("main user Pages 1 debut \n");
	PutChar('\n');
	int id= UserThreadCreate(f, (void *) THIS);
	f((void*) THAT);
	UserThreadJoin(id);

	return 0 ;
}
