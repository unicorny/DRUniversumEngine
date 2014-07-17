#ifndef __NETWORK_BUFFERED_CONNECTION__
#define __NETWORK_BUFFERED_CONNECTION__

//#include <UniversumLib.h>
#include "Connection.h"

class BufferedConnection: private Connection, UniLib::lib::Thread 
{
public:
	BufferedConnection(const char* url_host, int port, UniLib::lib::BufferedNetworkPacket* inputBuffer, UniLib::lib::BufferedNetworkPacket* outputBuffer);
	~BufferedConnection();

	virtual int ThreadFunction();
private:
	UniLib::lib::BufferedNetworkPacket* mInputBuffer;
	UniLib::lib::BufferedNetworkPacket* mOutputBuffer;
};


#endif //__NETWORK_BUFFERED_CONNECTION__