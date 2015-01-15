#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "machine.h"


static void StartUserThread(int f){



	typeArg * param = (typeArg*) f;

	ASSERT(param!=NULL);
	ASSERT(param->function!=0);
	ASSERT(param->arg!=0);

	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	//initialisation des registres

	machine->WriteRegister(PCReg,param->function);
	machine->WriteRegister(NextPCReg,(param->function)+4);
	machine->WriteRegister(4,param->arg);






	machine->Run();

}


int do_UserThreadCreate(int f, int arg)
{


	Thread *newThread  = new Thread("User Thread");

	if(newThread == NULL) {
		DEBUG('t',"Error while creating thread");
		return -1;
	}

	typeArg *args = new Arguments;
	args->function = f;
	args->arg = arg;

	newThread->space = currentThread->space;
	newThread->Fork(StartUserThread,(int)args);

	currentThread->Yield();

	//currentThread->Yield();

	return 0;
}


void do_UserThreadExit(){


	printf("in the exit\n");
	currentThread->Finish ();
}

#endif // CHANGED
