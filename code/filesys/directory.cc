// directory.cc 
//	Routines to manage a directory of file names.
//
//	The directory is a table of fixed length entries; each
//	entry represents a single file, and contains the file name,
//	and the location of the file header on disk.  The fixed size
//	of each directory entry means that we have the restriction
//	of a fixed maximum size for file names.
//
//	The constructor initializes an empty directory of a certain size;
//	we use ReadFrom/WriteBack to fetch the contents of the directory
//	from disk, and to write back any modifications back to disk.
//
//	Also, this implementation has the restriction that the size
//	of the directory cannot expand.  In other words, once all the
//	entries in the directory are used, no more files can be created.
//	Fixing this is one of the parts to the assignment.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "utility.h"
#include "filehdr.h"
#include "directory.h"

//----------------------------------------------------------------------
// Directory::Directory
// 	Initialize a directory; initially, the directory is completely
//	empty.  If the disk is being formatted, an empty directory
//	is all we need, but otherwise, we need to call FetchFrom in order
//	to initialize it from disk.
//
//	"size" is the number of entries in the directory
//----------------------------------------------------------------------

Directory::Directory(int size) {
	table = new DirectoryEntry[size];
	tableSize = size;
	for (int i = 0; i < tableSize; i++)
		table[i].inUse = FALSE;
#ifdef CHANGED
	int sector = 1;
	int parentSector = 1;
	linkToParentDirectory( sector , parentSector ) ;
#endif
}
#ifdef CHANGED
Directory::Directory(int size, int sector, int parentSector) {

	table = new DirectoryEntry[size];
	tableSize = size;
	for (int i = 2; i < tableSize; i++)
		table[i].inUse = FALSE;

	linkToParentDirectory( sector , parentSector ) ;
}
#endif

//----------------------------------------------------------------------
// Directory::~Directory
// 	De-allocate directory data structure.
//----------------------------------------------------------------------

Directory::~Directory() {
	delete[] table;
}

//----------------------------------------------------------------------
// Directory::FetchFrom
// 	Read the contents of the directory from disk.
//
//	"file" -- file containing the directory contents
//----------------------------------------------------------------------

void Directory::FetchFrom(OpenFile *file) {
	(void) file->ReadAt((char *) table, tableSize * sizeof(DirectoryEntry), 0);
}

//----------------------------------------------------------------------
// Directory::WriteBack
// 	Write any modifications to the directory back to disk
//
//	"file" -- file to contain the new directory contents
//----------------------------------------------------------------------

void Directory::WriteBack(OpenFile *file) {
	(void) file->WriteAt((char *) table, tableSize * sizeof(DirectoryEntry), 0);
}

//----------------------------------------------------------------------
// Directory::FindIndex
// 	Look up file name in directory, and return its location in the table of
//	directory entries.  Return -1 if the name isn't in the directory.
//
//	"name" -- the file name to look up
//----------------------------------------------------------------------

int Directory::FindIndex(const char *name) {
	for (int i = 0; i < tableSize; i++)
		if (table[i].inUse && !strncmp(table[i].name, name, FileNameMaxLen))
			return i;
	return -1; // name not in directory
}

//----------------------------------------------------------------------
// Directory::Find
// 	Look up file name in directory, and return the disk sector number
//	where the file's header is stored. Return -1 if the name isn't 
//	in the directory.
//
//	"name" -- the file name to look up
//----------------------------------------------------------------------

int Directory::Find(const char *name) {
	int i = FindIndex(name);

#ifdef CHANGED
	if(i==0||i==1){ // we can't delete the "." or the ".." Entry
		printf("permission denied, can't delete parent or current Directory !!\n");
		return (-1);
	}
#endif
	if (i != -1)
		return table[i].sector;

		return -1;


}

//----------------------------------------------------------------------
// Directory::Add
// 	Add a file into the directory.  Return TRUE if successful;
//	return FALSE if the file name is already in the directory, or if
//	the directory is completely full, and has no more space for
//	additional file names.
//
//	"name" -- the name of the file being added
//	"newSector" -- the disk sector containing the added file's header
//----------------------------------------------------------------------

bool Directory::Add(const char *name, int newSector) {
	if (FindIndex(name) != -1)
		return FALSE;

	for (int i = 0; i < tableSize; i++)
		if (!table[i].inUse) {
			table[i].inUse = TRUE;
			strncpy(table[i].name, name, FileNameMaxLen);
			table[i].sector = newSector;

			return TRUE;
		}
	return FALSE; // no space.  Fix when we have extensible files.
}

//----------------------------------------------------------------------
// Directory::Remove
// 	Remove a file name from the directory.  Return TRUE if successful;
//	return FALSE if the file isn't in the directory. 
//
//	"name" -- the file name to be removed
//----------------------------------------------------------------------

bool Directory::Remove(const char *name) {
	//char rep;
	int i = FindIndex(name);
	if (i == -1)
		return FALSE; // name not in directory

	table[i].inUse = FALSE;
	return TRUE;
}

//----------------------------------------------------------------------
// Directory::List
// 	List all the file names in the directory. 
//----------------------------------------------------------------------

void Directory::List() {
	for (int i = 0; i < tableSize; i++)
		if (table[i].inUse)
			printf("%s\n", table[i].name);
}

//----------------------------------------------------------------------
// Directory::Print
// 	List all the file names in the directory, their FileHeader locations,
//	and the contents of each file.  For debugging.
//----------------------------------------------------------------------

void Directory::Print() {
	FileHeader *hdr = new FileHeader;

	printf("Directory contents:\n");
	for (int i = 0; i < tableSize; i++)
		if (table[i].inUse) {
			printf("Name: %s, Sector: %d\n", table[i].name, table[i].sector);
			hdr->FetchFrom(table[i].sector);
			hdr->Print();
		}
	printf("\n");
	delete hdr;
}

///////////////////////////////////////////////////////////////////////////////////////////
#ifdef CHANGED
void Directory::linkToParentDirectory(int sector, int parentSector){

	table[0].inUse = true;
	table[0].sector = sector;
	strcpy(table[0].name, ".");
	table[1].inUse = true;
	table[1].sector = parentSector;
	strcpy(table[1].name, "..");
}

////////////////////////////////////////////////////////////////////////////////////////
int Directory::getSector(int position) {

	return this->table[position].sector;

}
/////////////////////////////////////////////////////////////////////////////////////////
int Directory::getCurrentSector(){
	Directory *currentDir = this;

	return currentDir->getSector(0);
}
///////////////////////////////////////////////////////////////////////////////////////////
int Directory::getParentSector(){
	Directory *currentDir = this;
	return currentDir->getSector(1);
}
///////////////////////////////////////////////////////////////////////////////////////////
char * Directory::getNameFromSector(int sector) {
	Directory *currentDir = this;
	int i;
	for (i = 2; i < tableSize; i++)
	if (currentDir->getSector(i)== sector) {
		return currentDir->table[i].name;
	}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////
bool Directory::isFull() {
	Directory *currentDir = this;
	int i;
	for ( i = 2; i < tableSize; i++) {
		if(currentDir->table[i].inUse == false)
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool Directory::isEmpty() {
	Directory *currentDir = this;
	int i;
	for ( i = 2; i < tableSize; i++) {
		if(currentDir->table[i].inUse == true)
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool Directory::isRoot() {

	// ". == ".."
	return (this->getSector(0) == this->getSector(1));
};
//////////////////////////////////////////////////////////////////////////////////////////
char *Directory::getDirName() {
	Directory * currentDir = this;
	Directory * parentDir;
	int parentSector;
	char * fullname = new char[FileNameMaxLen];
	char * temp = new char[FileNameMaxLen];
	strcpy(fullname, "/");
	char * currentName = new char [FileNameMaxLen];
	while (!currentDir->isRoot()) {
		parentSector = currentDir->getParentSector();
		OpenFile * parentDirFile = new OpenFile(parentSector);
		parentDir = new Directory(this->tableSize);
		parentDir->FetchFrom(parentDirFile);
		currentName = parentDir->getNameFromSector(currentDir->getCurrentSector());
		strcpy(temp, "/");
		strcat(temp, currentName);
		strcat(temp, fullname);
		strcpy(fullname, temp);
		currentDir = parentDir;
	}
	return fullname;
}

#endif
