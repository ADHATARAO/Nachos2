#include "syscall.h"

void print(void * arg) {

	int num = (int)arg;

	PutString("In the print function : \n");
	PutInt(num);
	PutChar('\n');
//	UserThreadExit();
}

int main() {

	int i =10;

	UserThreadCreate(print, (void *) &i);
	//UserThreadCreate(print, (void *) j);

	return 0;
}

