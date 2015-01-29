#include "syscall.h"

int main() {

	char buffer[100];
	GetString(buffer, 100);
	PutString("\nYou typed : ");
	PutString(buffer);
	PutString("\n");
	Halt();
}
