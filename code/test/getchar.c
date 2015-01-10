#include "syscall.h"

int main() {
	 char c = GetChar();

//	 char c = 'c';
//	  PutString("You entered : ");
	  PutChar(c);
    Halt();
}
