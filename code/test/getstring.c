#include "syscall.h"

int main() {

	char buffer[100];
	GetString(buffer, 100);
	PutString("\n You typed : ");
	PutString(buffer);
	Halt();
}
