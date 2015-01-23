#include "syscall.h"

void print(void * arg) {
	char* ch = (char*) arg;

	PutString("In the print function : \n");
	PutString(ch);
	PutChar('\n');
//	UserThreadExit();
}

int main() {

	PutString("In the main function : \n");
	char ch[] = "hello";
	char ch1[] = "salut";
	char ch2[] = "nooo";

	int id1 = UserThreadCreate(print, (void *) ch);
	PutString("In the main function une autre fois : \n");
	int id2 = UserThreadCreate(print, (void *) ch1);
	int id3 = UserThreadCreate(print, (void *) ch2);


	UserThreadJoin(id1);
	PutString("Join >>"); PutInt(id1);
	UserThreadJoin(id2);
	PutString("Join >>"); PutInt(id2);
	UserThreadJoin(id3);
	PutString("Join >>"); PutInt(id3);

	PutString("In the main function une autre fois aussi :p: \n");
	PutChar('\n');
	return 0;
}

