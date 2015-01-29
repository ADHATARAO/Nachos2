#ifdef CHANGED
#include "copyright.h"

#ifndef RELIABLE_POST_H
#define RELIABLE_POST_H

#include "network.h"
#include "synchlist.h"
#include "post.h"


#define TEMPO 50000000
#define MAXREEMISSIONS 3

struct RMailHeader {
    int serialNum;
    int ack;
};

class ReliablePost{
	public:
		void Send(PacketHeader pktHdr, MailHeader mailHdr, const char *data);
		void Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, char *data);
};

#endif //RELIABLE_POST_H
#endif //CHANGED
