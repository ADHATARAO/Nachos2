#ifdef CHANGED
#include "userthread.h"
#include "system.h"

Semaphore *threadCounterLock = new Semaphore("Thread Counter Lock",1); // to protect the counter
Semaphore *threadIdLock = new Semaphore("Thread Id Lock",1);// to make sure that

int totalThreadNum = 1;//to keep track of number of threads created
static int threadCounter = 1;// to assign the thread id

#define MAX_THREAD_NUM (int)(UserStackSize/(PagePerThread*PageSize))
static Semaphore* waitThread[MAX_THREAD_NUM]; // used to join two threads

/*
 // initialize registers and run the thread
 */
static void StartUserThread(int f)
{
	typeArg * param = (typeArg*) f;

	ASSERT(param!=NULL);
	ASSERT(param->function!=0);
	ASSERT(param->arg!=0);

	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();

	machine->WriteRegister(RetAddrReg,param->call_back);

	machine->WriteRegister(PCReg,param->function);
	machine->WriteRegister(NextPCReg,(param->function)+4);
	machine->WriteRegister(4,param->arg);
	machine->WriteRegister(StackReg,currentThread->space->BeginPointStack());

	machine->Run();
}

// Create a new thread and assign it a thread id
int do_UserThreadCreate(int f, int arg, int call_back)
{
	if(getThreadNum() == MAX_THREAD_NUM)
	{
		DEBUG('t',"Error while creating thread : MAX THREAD NUM exceeded ");
		return -1;
	}

	Thread *newThread = new Thread("User Thread");

	if(newThread == NULL) {
		DEBUG('t',"Error while creating thread");
		return -1;
	}

	newThread->SetThreadId(assignThreadId());
	int id = newThread->GetThreadId();

	increaseThreadNum();

	waitThread[id] = new Semaphore("wait executing other thread",0);

	DEBUG('t',"New thread created with thread ID >> %d", id);

	typeArg *args = new Arguments;
	args->function = f;
	args->arg = arg;
	args->id = id;
	args->call_back = call_back;

	newThread->Fork(StartUserThread,(int)args);
	currentThread->Yield();
	return id;
}

// increment the counter then  assign a thread id
int assignThreadId()
{
	threadIdLock->P();
	int id = threadCounter++;
	threadIdLock->V();

	return id;
}

// update the bitmap and release the semaphore then terminate the user thread
void do_UserThreadExit()
{
	DEBUG('t',"Deleting thread ID >> %d", currentThread->GetThreadId());

	decreaseThreadNum();
	FreeThread(currentThread->GetThreadId());
	currentThread->space->DealloateMapStack();
	currentThread->Finish ();
}

//lock the semaphore in order to wait for the thread whose id is 'idThread'
void do_UserThreadJoin(int idThread)
{
	ASSERT(idThread!=0); // make sure that the thread joined is not main thread
	LockThread(idThread);
	delete waitThread[idThread];
}

// increment the thread counter
void increaseThreadNum()
{
	threadCounterLock->P();
	totalThreadNum++;
	threadCounterLock->V();
}

// decrement the thread counter
void decreaseThreadNum()
{
	threadCounterLock->P();
	totalThreadNum--;
	threadCounterLock->V();
}

// to get the total number of threads
int getThreadNum()
{
	return totalThreadNum;
}

// in order to wait for a thread
void LockThread(int id)
{
	waitThread[id]->P();
}

// in order to release a thread
void FreeThread(int id)
{
	waitThread[id]->V();
}

#endif // CHANGED
