#ifdef CHANGED

#include "copyright.h"
#include "reliablepost.h"

#include <strings.h> /* for bzero */
#include "thread.h"
#include "system.h"

int sn_num = 1;
static Semaphore *snLock= new Semaphore("Serial number Lock",1);
static Semaphore *waitLock= new Semaphore("Send Lock",1);


/*ReliablePost::ReliablePost(int nBoxes)
{
	boxes = new MailBox[nBoxes];
}

ReliablePost::~ReliablePost()
{
	delete [] boxes;
}*/

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
	RMailHeader outmailHdr;

	outmailHdr.serialNum = assignSN();
	outmailHdr.ack = 0;

	char* buffer = new char[MaxPacketSize];
	char* newbuffer = new char[MaxPacketSize];

	printf("sizeof(RMailHeader) >>>> %d \n", sizeof(RMailHeader));
	printf("mailHdr.length>>>> %d \n", mailHdr.length);


	bcopy(&outmailHdr, buffer, sizeof(RMailHeader));
	bcopy(data, buffer + sizeof(RMailHeader), mailHdr.length);
	mailHdr.length +=  sizeof(RMailHeader);


	printf("after sizeof(RMailHeader) >>>> %d \n", sizeof(RMailHeader));
	printf("after mailHdr.length>>>> %d \n", mailHdr.length);

	printf("buffer >>>> %d \n",*((int*)buffer) );
	printf("buffer 1 >>>> %d \n",*((bool*)(buffer + 4)));
	/*printf("serial number >> %d \n",outmailHdr.serialNum);
	printf("outmailHdr.ack >> %d \n",outmailHdr.ack);
*/

	for(int i= 0 ; i <MAXREEMISSIONS ; i ++){
		postOffice->Send(pktHdr, mailHdr, data);

		interrupt->Schedule(waitRespondLock,(int)this, TEMPO, NetworkRecvInt);
		waitLock->P();
		if(postOffice->CheckMailBox(0)){
			printf("inside checkmail box if >> %d\n", postOffice->CheckMailBox(0));
			Receive(0, &pktHdr, &mailHdr, newbuffer);
			break;
		}
		// if there is a msg into the mail box
		// else
	}
}

void
ReliablePost::Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, char *data)
{

	char* buffer = new char[MaxPacketSize];
	postOffice->Receive(0, pktHdr, mailHdr, buffer);

	printf("buffer <<<< %d \n",*((int*)buffer) );
	printf("buffer + 1 <<<< %d \n",*((int*)buffer + 1));

	RMailHeader inmailHdr;

	printf("Receive sizeof(RMailHeader) >>>> %d \n", sizeof(RMailHeader));
	printf(" Receive mailHdr.length>>>> %d \n", mailHdr->length);




	bcopy(buffer, &inmailHdr, sizeof(RMailHeader)); // copying content of data into inmailHdr
	bcopy(buffer + sizeof(RMailHeader), data , mailHdr->length); // copying contains main data only into buffer

	mailHdr->length -= sizeof(RMailHeader);

	inmailHdr.ack = 1;






	printf(" Receive serial number >> %d \n",inmailHdr.serialNum);
	printf(" Receive outmailHdr.ack >> %d \n",inmailHdr.ack);

	printf("after Receive sizeof(RMailHeader) >>>> %d \n", sizeof(RMailHeader));
	printf(" after Receive mailHdr.length>>>> %d \n", mailHdr->length);
	printf("after data >> %s \n", buffer );
}
#endif //CHANGED
