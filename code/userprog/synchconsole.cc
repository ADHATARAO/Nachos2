#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg) {readAvail->V();}
static void WriteDone(int arg) {writeDone->V();}

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar(ch); // echo it!
	writeDone->P();
}

char SynchConsole::SynchGetChar()
{
	char ch;
	readAvail->P(); // wait for character to arrive
	ch = console->GetChar();

	return ch;
}

void SynchConsole::SynchPutString(const char s[])
{
	for(int i=0; i<MAX_STRING_SIZE-1;i++) {
		if(s[i] == '\0')
			break;
		this->SynchPutChar(s[i]);
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	int i;
	char c;
	for (i=0; i<n-1; i++) {
		c = this->SynchGetChar();

		if(c == EOF || c =='\n')
			break;
		else
			s[i] = c;
	}
	s[i] = '\0';
}

void SynchConsole::SynchPutInt(int value) {
	char * buffer = new char[MAX_STRING_SIZE];
	snprintf(buffer,MAX_STRING_SIZE, "%d", value);
	this->SynchPutString(buffer);
	delete [] buffer;
}


void SynchConsole::SynchGetInt(int *n){
	char *buffer = new char[MAX_STRING_SIZE];
	this->SynchGetString(buffer, MAX_STRING_SIZE);
	sscanf(buffer,"%i",n);
	delete [] buffer;
}

#endif // CHANGED
