#include "syscall.h"

int main() {
	int i;

	GetInt(&i);
	PutInt(i);
	PutChar('\n');

	return 0;
}

