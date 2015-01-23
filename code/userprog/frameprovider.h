#ifdef CHANGED
#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "bitmap.h"
class FrameProvider {
public:
	FrameProvider();
	~FrameProvider();
	int GetEmptyFrame(); // to get empty frame
	void ReleaseFrame(int n); // to release a frame
	int NumAvailFrame(); // to get number of available frames
private:
	BitMap* frame;
};


#endif //FRAMEPROVIDER_H
#endif// CHANGED
