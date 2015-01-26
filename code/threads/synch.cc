// synch.cc 
//      Routines for synchronizing threads.  Three kinds of
//      synchronization routines are defined here: semaphores, locks 
//      and condition variables (the implementation of the last two
//      are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      Initialize a semaphore, so that it can be used for synchronization.
//
//      "debugName" is an arbitrary name, useful for debugging.
//      "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore (const char *debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      De-allocate semaphore, when no longer needed.  Assume no one
//      is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore ()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//      Wait until semaphore value > 0, then decrement.  Checking the
//      value and decrementing must be done atomically, so we
//      need to disable interrupts before checking the value.
//
//      Note that Thread::Sleep assumes that interrupts are disabled
//      when it is called.
//----------------------------------------------------------------------

void
Semaphore::P ()
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff);	// disable interrupts

    while (value == 0)
      {				// semaphore not available
	  queue->Append ((void *) currentThread);	// so go to sleep
	  currentThread->Sleep ();
      }
    value--;			// semaphore available, 
    // consume its value

    (void) interrupt->SetLevel (oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//      Increment semaphore value, waking up a waiter if necessary.
//      As with P(), this operation must be atomic, so we need to disable
//      interrupts.  Scheduler::ReadyToRun() assumes that threads
//      are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V ()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel (IntOff);

    thread = (Thread *) queue->Remove ();
    if (thread != NULL)		// make thread ready, consuming the V immediately
	scheduler->ReadyToRun (thread);
    value++;
    (void) interrupt->SetLevel (oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!

//-------------------------------------------------
//We implement locks using semaphores
//-------------------------------------------------
Lock::Lock (const char *debugName)
{   
#ifdef CHANGED
    name = debugName;
     mutex = new Semaphore(debugName, 1); //lock is available
#endif //CHANGED
}

//---------------------------------------------------
//Lock destructor deletes locks
//---------------------------------------------------
Lock::~Lock ()
{   
#ifdef CHANGED
    delete mutex;
#endif //CHANGED
}

//--------------------------------------------------
//Wait until lock is available , then get lock
//--------------------------------------------------
void
Lock::Acquire ()
{   
#ifdef CHANGED
    mutex->P();//if lock is free, take lock else wait
#endif //CHANGED
}

//------------------------------------------------------
//wake up threads waiting on lock once lock is released

//------------------------------------------------------
void
Lock::Release ()
{   
#ifdef CHANGED
    mutex->V();// release lock and wake up threads waiting on lock
#endif //CHANGED
}
//-----------------------------------------------
//Condition variables on which threads may wait
//-----------------------------------------------
Condition::Condition (const char *debugName)
{  
 #ifdef CHANGED
    condvarwait = new Semaphore(debugName, 0); // Nobody waiting on condition variable
    //condsig = new Semaphore(debugName, 1);//Lock is available
    threadwait = 0; //no thread is waiting
#endif //CHANGED
}
//----------------------------------------------
//Condition destructor deletes semaphores
//---------------------------------------------
Condition::~Condition ()
{   
#ifdef CHANGED
    delete condvarwait;
    //delete condsig;
#endif //CHANGED
}
//--------------------------------------------------
//release lock
//release CPU
//re-acquire lock
//--------------------------------------------------
void
Condition::Wait (Lock * conditionLock)
{
#ifdef CHANGED
    threadwait += 1;//increment waiting threads
    conditionLock->Release(); // release the lock
    condvarwait->P(); // Put thread to sleep until signaled
   // condsig->V();//Signal sleeping thread
    conditionLock->Acquire();//get lock
#endif //CHANGED
}
//-----------------------------------
//search list of threads waiting
//signal first thread connected with the semaphore
//-----------------------------------
void
Condition::Signal (Lock * conditionLock)
{   
#ifdef CHANGED
    //condsig = new Semaphore(name, 1);
    if (threadwait > 0) { // don't signal if no thread is waiting
        threadwait--; //remove from list of waiting threads
        condvarwait->V(); // wake up thread connected with semaphore
        //condsig->P(); // Semaphore Waits
    }
    //delete condsig;
#endif //CHANGED
}

//-----------------------------------------------
//wake up all waiting threads upon execution
//-----------------------------------------------
void
Condition::Broadcast (Lock * conditionLock)
{ 
#ifdef CHANGED

    /*for(int i =0; i< threadwait; i++)
        condvarwait->V();*/
    while (threadwait > 0)//don't broadcast if no thread is waiting 
    {
        //threadwait--;
        condvarwait->V();// wake up all threads waiting
        //condsig->P();
    }

#endif //CHANGED
}