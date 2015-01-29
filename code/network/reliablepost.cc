#ifdef CHANGED

#include "copyright.h"
#include "reliablepost.h"

#include <strings.h> /* for bzero */
#include "thread.h"
#include "system.h"

int sn_num = 1;
static Semaphore *snLock= new Semaphore("Serial number Lock",1);
static Semaphore *waitLock= new Semaphore("Send Lock",1);



static int assignSN()
{
	snLock->P();
	int sn = sn_num++;
	snLock->V();

	return sn;
}

static void waitRespondLock(int arg)
{
	waitLock->V();
}

void
ReliablePost::Send(PacketHeader pktHdr, MailHeader mailHdr, const char *data)
{

/*
	printf("pktHdr.to >> %d \n ", pktHdr.to);
	printf("mailHdr.to >> %d \n ", mailHdr.to);
	printf("mailHdr.from >> %d \n ", mailHdr.from);
*/
	RMailHeader outmailHdr;
	outmailHdr.serialNum = assignSN();
	outmailHdr.ack = 0;

	char* buffer = new char[MaxPacketSize];

	bcopy(&outmailHdr, buffer, sizeof(RMailHeader));
	bcopy(data, buffer + sizeof(RMailHeader), mailHdr.length);
	mailHdr.length += sizeof(RMailHeader);

	char* newbuffer = new char[MaxPacketSize];
	bool sucess = false;

	for(int i = 0; i <MAXREEMISSIONS &&  !sucess; i ++)
	{
		postOffice->Send(pktHdr, mailHdr, buffer);

		interrupt->Schedule(waitRespondLock,(int)this, TEMPO, NetworkRecvInt);
		waitLock->P();

		// if there is a msg into the mail box
		if(!postOffice->EmptyMailBox(pktHdr.to) == false)
		{
			printf("inside if #%d \n",i );

			Receive(0, &pktHdr, &mailHdr, newbuffer);

			printf("new buffer data >> %s \n", newbuffer);
			sucess = true;
			break;
		}else{
			printf("sending again #%d \n",i );
		}
	} // end of for loop
}

void
ReliablePost::Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, char *data)
{
	printf("inside Receive function \n");

	RMailHeader inmailHdr;

	char* buffer = new char[MaxPacketSize];

	printf("inside Receive function >> print empty buffer \n");

	postOffice->Receive(0, pktHdr, mailHdr, buffer);




	bcopy(buffer, &inmailHdr, sizeof(RMailHeader)); // copying content of data into inmailHdr
	bcopy(buffer + sizeof(RMailHeader), data, mailHdr->length); // copying contains main data only into buffer

	mailHdr->length -= sizeof(RMailHeader);


	printf("inside Receive function >> return data >> %s \n", data);

	/*Sending acknowledgement back*/
	char* ackbuffer = new char[MaxPacketSize];
	inmailHdr.ack = 1;
	bcopy(&inmailHdr, ackbuffer, sizeof(RMailHeader));

	PacketHeader outPktHdr;
	MailHeader outMailHdr;

	outPktHdr.to = pktHdr->from;
	outMailHdr.to = mailHdr->from;
	outMailHdr.from = mailHdr->to;
	outMailHdr.length = sizeof(RMailHeader);

	postOffice->Send(outPktHdr, outMailHdr, ackbuffer);
}

#endif //CHANGED
