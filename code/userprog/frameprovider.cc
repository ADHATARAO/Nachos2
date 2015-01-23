#ifdef CHANGED
#include "frameprovider.h"
#include "system.h"
#include <strings.h>

FrameProvider::FrameProvider() {
	frame = new BitMap(NumPhysPages);
}

FrameProvider::~FrameProvider() {
	delete frame;
}

int FrameProvider::GetEmptyFrame() {
	int find = frame->Find();
	ASSERT(find!=-1)

	bzero(&machine->mainMemory[find*PageSize], PageSize);
	return find;
}

void FrameProvider::ReleaseFrame(int n) {
	frame->Clear(n);
}

int FrameProvider::NumAvailFrame() {
	return frame->NumClear();
}

#endif //CHANGED
