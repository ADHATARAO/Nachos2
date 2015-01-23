#ifdef CHANGED
#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "thread.h"
#include "synch.h"

// use a struct to pass only one argument to the fork function
typedef struct Arguments{
	int function;
	int arg;
	int id;
	int call_back;

} typeArg;

void increaseThreadNum();
void decreaseThreadNum();
int getThreadNum();

extern int do_UserThreadCreate(int f, int arg, int call_back);
extern void do_UserThreadExit();
extern void do_UserThreadJoin(int idThread);
extern int assignThreadId();

void LockThread(int id);
void FreeThread(int id);

#endif //USERTHREAD_H
#endif // CHANGED
