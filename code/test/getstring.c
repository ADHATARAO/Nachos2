#include "syscall.h"

int main() {

	char buffer[17];
	GetString(buffer, 17);
	PutString("\n You typed : ");
	PutString(buffer);
	PutString("\n");
	Halt();
}
