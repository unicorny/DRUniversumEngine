#ifndef __NETWORK_BUFFERED_CONNECTION__
#define __NETWORK_BUFFERED_CONNECTION__

//#include <UniversumLib.h>
#include "Connection.h"

using namespace UniLib;
using namespace lib;

#define BUFFERED_CONNECTION_RERUN_DELAY_MILLISECONDS 1000
#define BUFFERED_CONNECTION_RECIVE_BUFFER_SIZE_BYTES 1024
#define BUFFERED_CONNECTION_MAX_RECIVE_DATA_BLOCK_MBYTES 10
#define BUFFERED_CONNECTION_MAX_RECIVE_DATA_BLOCK_KBYTES BUFFERED_CONNECTION_MAX_RECIVE_DATA_BLOCK_MBYTES * 1024
#define BUFFERED_CONNECTION_MAX_RECIVE_DATA_BLOCK_BYTES BUFFERED_CONNECTION_MAX_RECIVE_DATA_BLOCK_KBYTES * 1024

class BufferedConnection: private Connection, public UniLib::lib::TimingThread 
{
public:
	BufferedConnection(std::string name, Timer* timer, const char* url_host, int port,
					   BufferedNetworkPacket* inputBuffer, BufferedNetworkPacket* outputBuffer);
	~BufferedConnection();

	virtual int ThreadFunction();

private:
	DRReturn recv();
	DRReturn send();

	BufferedNetworkPacket* mInputBuffer;
	BufferedNetworkPacket* mOutputBuffer;

	struct RecivingBuffer {
		RecivingBuffer() :emptyBytes(BUFFERED_CONNECTION_RECIVE_BUFFER_SIZE_BYTES-1), readedBytes(0) {
			memset(buffer, 0, BUFFERED_CONNECTION_RECIVE_BUFFER_SIZE_BYTES);
		}
		int recived(int recivedBytes) {
			readedBytes += recivedBytes;
			emptyBytes -= recivedBytes;
			return emptyBytes;
		}
		void print(FILE* f = NULL)
		{
			printf("[RecivingBuffer::print] readed: %d, content: %s\n", readedBytes, buffer);
			//DRLog.writeToLogDirect("<pre>%s</pre>", buffer);
			if(f) {
				fprintf(f, buffer);
			}

		}
		char buffer[BUFFERED_CONNECTION_RECIVE_BUFFER_SIZE_BYTES];
		int emptyBytes;
		int readedBytes;
	};
};


#endif //__NETWORK_BUFFERED_CONNECTION__