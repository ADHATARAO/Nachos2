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

#ifdef CHANGED
#include "fork.h"
#include "userthread.h"

#endif //CHANGED
//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void UpdatePC() {
	int pc = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, pc);
	pc = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, pc);
	pc += 4;
	machine->WriteRegister(NextPCReg, pc);
}

#ifdef CHANGED
// in this function we read from the location specified by the first argument
//and store it in the location specified by the second argument
// the number of bytes to be read is specified by the third argument
void copyStringFromMachine(int from, char *to, unsigned size)
{
	int byte;
	unsigned int i;
	int offset= 0;

	do {
		for (i = 0; i < size - 1; i++) {
			machine->ReadMem(from + offset + i, 1, &byte);

			if ((char) byte == '\0')
			break;
			to[i] = (char) byte;
		}

		offset = offset + i;
		to[i] = '\0';

	}while ((char) byte != '\0');
}

// in this function we write to the location specified by the second argument
//and store it in the location specified by the first argument
// the number of bytes to be read is specified by the third argument
void writeStringToMachine(char * string, int to, unsigned size)
{
	int i = 0;

	for (i = 0; i < (int)size; i++)
	{
		machine->WriteMem(to + i, 1, string[i]);
	}
}
#endif //CHANGED
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

void ExceptionHandler(ExceptionType which) {
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


				int numProcess = machine->GetNbProcess(); // counting total number of processes

				if(numProcess > 1) {
					machine->SetNbProcess(numProcess - 1);	// decrement the number of processes until it's one
					currentThread->Finish();				// terminate the process
				} else {
					interrupt->Halt();						// the last process Halts the machine
				}
				break;
			}
			case SC_Exit: {
				DEBUG('a', "Program exit.\n");

			    int numProcess = machine->GetNbProcess();

				if(numProcess > 1) {
					machine->SetNbProcess(numProcess - 1);
					currentThread->Finish();
				} else {
					interrupt->Halt();
				}
				break;

			}
			case SC_PutChar: { // to write a character to the console
				DEBUG('m', "PutChar, system call handler. \n");

				int charint = machine->ReadRegister(4);
				char ch = (char) charint;
				synchConsole->SynchPutChar(ch);
				break;
			}
			case SC_PutString: { // to write a string to the console
				DEBUG('m', "PutString, system call handler.\n");

				char *buffer = new char[MAX_STRING_SIZE];
				copyStringFromMachine(machine->ReadRegister(4), buffer, MAX_STRING_SIZE);
				synchConsole->SynchPutString(buffer);
				delete [] buffer;
				break;
			}
			case SC_GetChar: { // to read a character from the console
				DEBUG('m', "GetChar, system call handler.\n");

				machine->WriteRegister(2,(int) synchConsole->SynchGetChar());
				break;
			}
			case SC_GetString: { // to read a String from the console
				DEBUG('m', "GetString, system call handler.\n");

				char *buffer = new char[MAX_STRING_SIZE+1];
				int reg5 = machine->ReadRegister(5), p = 0, size;
				while (p < reg5 ) {
					if (reg5 - p > MAX_STRING_SIZE+1)
					size = MAX_STRING_SIZE+1;
					else
					size = reg5 - p;
					synchConsole->SynchGetString(buffer, size);
					writeStringToMachine(buffer, machine->ReadRegister(4)+p, strlen(buffer));
					p+= strlen(buffer);

					if (strlen(buffer) != MAX_STRING_SIZE)
					break;
				}
				machine->WriteMem(machine->ReadRegister(4)+p+1, 1, '\0');
				delete [] buffer;
				break;
			}
			case SC_PutInt: { // to write an int to the Console
				DEBUG('m', "PutInt, system call handler.\n");

				int value = machine->ReadRegister(4);
				synchConsole->SynchPutInt(value);
				break;
			}
			case SC_GetInt: {	// to read an int from the console
				DEBUG('m', "GetInt, system call handler.\n");

				int num;
				synchConsole->SynchGetInt(&num);
				int adr = machine->ReadRegister(4);
				machine->WriteMem(adr,4,num);
				break;
			}
			case SC_UserThreadCreate: {	// to create a user Thread
				DEBUG('t', "UserThreadCreate, initiated by user program.\n");

				int f = machine->ReadRegister(4);
				int arg = machine->ReadRegister(5);
				int result = do_UserThreadCreate(f,arg);

				machine->WriteRegister(2,result);
				break;
			}
			case SC_UserThreadExit:	{ // to terminate a user thread

				DEBUG('t', "UserThreadExit, initiated by user program.\n");

				do_UserThreadExit();
				break;
			}
			case SC_UserThreadJoin:{ // to join a user thread

				int idThread = machine->ReadRegister(4);
				do_UserThreadJoin(idThread);
				break;
			}
			case SC_ForkExec:{	// to fork new process

				int arg = machine->ReadRegister(4);
				char* to = new char[MAX_STRING_SIZE+1]; // the +1 used to write the EOF character '\0'
				copyStringFromMachine(arg, to, MAX_STRING_SIZE);

				//printf("%s\n",to);
				int res = do_UserFork(to);
				machine->WriteRegister(2,res);
				ASSERT(res==0);

				break;
			}
			default: {

				break;

			}
		} // end of switch case
	} // end of if statement
	else
	{
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
	}

#endif // CHANGED
	UpdatePC();
}
