#ifdef CHANGED
#include "fork.h"
#include "thread.h"
#include "addrspace.h"
#include "synch.h"
#include "system.h"
#include "console.h"

//structure for each process which will be forked
//inorder to use separate variable for each process
struct Serialisation
{
	AddrSpace* space;
};


static void DoStartProcess(int arg)
{
	Serialisation* restor = (Serialisation*) arg;

	currentThread->space = restor->space;
	currentThread->space->InitRegisters ();
	currentThread->space->RestoreState ();

	machine->SetNbProcess(machine->GetNbProcess()+1);// increment the number of processes forked
	machine->Run ();
}

int do_UserFork(char *s)
{
	 OpenFile *executable = fileSystem->Open (s);//opens the file specified
	 //printf ("opening file %s\n", s);
	 if (executable == NULL)						//if does not exists or unable to open then returns -1
	 {
		  printf ("Unable to open file %s\n", s);
		  return -1;
	 }

	AddrSpace *space = new AddrSpace(executable);
	delete executable;

	Thread* newThread = new Thread("newProcess");
	Serialisation* save = new Serialisation;
	//AddrSpace* space = new AddrSpace;

	save->space = space;
	newThread->Fork(DoStartProcess,(int)save);			//creating a new process


	currentThread->Yield();
	return 0;
}
#endif //CHANGED
