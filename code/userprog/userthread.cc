#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "machine.h"


static void StartUserThread(int f){

	printf("in the StartUserThread\n");

	typeArg * param = (typeArg*) f;

	ASSERT(param!=NULL);
	ASSERT(param->function!=0);
	ASSERT(param->arg!=0);

	machine->WriteRegister(PCReg,param->function);
	machine->WriteRegister(NextPCReg,(param->function)+4);

	machine->WriteRegister(4,param->arg);

	currentThread->space->WriteSpReg();


	printf("in the startuserthread\n");


	machine->Run();
}


int do_UserThreadCreate(int f, int arg)
{
	printf("in the do_UserThreadCreate\n");

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

	printf("in the do_UserThreadCreate below\n");

//	currentThread->Yield();
	return 0;
}


void do_UserThreadExit(){
//
}

#endif // CHANGED
