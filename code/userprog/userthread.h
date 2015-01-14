#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "system.h"
#include "syscall.h"

typedef struct Arguments{
	int function;
	int arg;
} typeArg;

extern int do_UserThreadCreate(int f, int arg);
extern void do_UserThreadExit();

#endif
