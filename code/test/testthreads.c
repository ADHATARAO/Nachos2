#include "syscall.h"

void thread (void *arg) {

	int i;

  for (i = 0; i < 10; i++) {
	  PutString((char *)arg);
	  PutChar('\n');
	  PutInt(i);
	  PutChar('\n');
  }

  UserThreadExit();
}


int main()
{
  PutInt(987654321);
  PutChar('\n');

  int id = UserThreadCreate(thread, "cccc\n");
  UserThreadJoin(id);
  return 0;
}
