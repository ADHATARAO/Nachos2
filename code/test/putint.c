#include "syscall.h"

int main() {
	int i;
	for (i = 0; i < 5; i++) {
		PutInt(i);
		PutChar('\n');
	}
	return 0;
}

