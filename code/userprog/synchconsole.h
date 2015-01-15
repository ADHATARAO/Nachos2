#ifdef CHANGED

#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"
#include "synch.h"


class SynchConsole {
public:
	SynchConsole(char *readFile, char *writeFile);
// initialize the hardware console device
	~SynchConsole(); 	// clean up console emulation

	void SynchPutChar(const char ch);	// Unix putchar(3S)
	char SynchGetChar(); 	// Unix getchar(3S)

	void SynchPutString(const char *s);		// Unix puts(3S)
	void SynchGetString(char *s, int n);	// Unix fgets(3S)
	void SynchPutInt(int value);
	void SynchGetInt(int *n);

private:
	Console *console;
	Semaphore *writeChar;
	Semaphore *readChar;
	Semaphore *writeString;
	Semaphore *readString;
};
#endif // SYNCHCONSOLE_H

#endif // CHANGED
