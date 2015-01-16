#include "syscall.h"

void print(void * arg) {
	char* ch = (char*) arg;
	while (1 == 1) {
		PutString(ch);
		PutChar('\n');

	}
	UserThreadExit();
}

int main() {

	PutString("In the main function : \n");
	char ch[] = "hello";
	char ch1[] = "salut";
	UserThreadCreate(print, (void *) ch);
	PutString("In the main function une autre fois : \n");
	UserThreadCreate(print, (void *) ch1);
	PutString("In the main function une autre fois aussi :p: \n");

	return 0;
}

