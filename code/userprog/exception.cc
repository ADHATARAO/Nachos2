// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

void copyStringFromMachine(int from, char *to, unsigned size)
{
	int value;
	unsigned int i;

	for (i = 0; i < size - 1; i++) {
		machine->ReadMem(from + i, 1, &value);
		if ((char) value == '\0')
			break;
		to[i] = (char) value;
	}
	to[i] = '\0';

	synchConsole->SynchPutString(to);
}
void WriteStringToMachine(char * string, int to, unsigned max_size) {
/* copy byte by byte with a casting  */
	char * bytes = (char *)(&machine->mainMemory[to]);
	for(unsigned int i = 0; i < max_size-1; i++) {
			bytes[i] = string[i];
			if(string[i]=='\0')
				break;
	}
}
//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

	#ifndef CHANGED // Noter le if*n*def
		if ((which == SyscallException) && (type == SC_Halt)) {
			DEBUG('a', "Shutdown, initiated by user program.\n");
			interrupt->Halt();
		} else {
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
		}
	#else // CHANGED

	if (which == SyscallException)
	{
		switch (type) {
			case SC_Halt: {
				DEBUG('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt();
				break;
			}
			case SC_PutChar: {
				int charint = machine->ReadRegister(4); //The compiler puts the first argument char c in the r4 register
				char ch = (char) charint;
				synchConsole->SynchPutChar(ch);
				break;
			}
			case SC_PutString: {
				char *buffer = new char[MAX_STRING_SIZE];
				copyStringFromMachine(machine->ReadRegister(4), buffer, MAX_STRING_SIZE);
				break;
			}
			case SC_GetChar: {
				machine->WriteRegister(2,(int) synchConsole->SynchGetChar());
				break;
			}
			case SC_GetString: {
			// le premier argument est une adresse (char *)
			int to = machine->ReadRegister(4);
			// le second est un int >> la taille
			int size = machine->ReadRegister(5);
			// On donne pas acceder à la mémoire directement,on ecrit  dans
			// un buffer..
			char buffer[MAX_STRING_SIZE];
			synchConsole->SynchGetString(buffer, size);
			WriteStringToMachine(buffer, to, size);
			break;
			}

			default: {
				printf("Unexpected user mode exception %d %d\n", which, type);
				ASSERT(FALSE);
				break;
			}
		}
		UpdatePC();
	}
}
#endif // CHANGED
