#include "BufferedConnection.h"

BufferedConnection::BufferedConnection(const char* url_host, int port, UniLib::lib::BufferedNetworkPacket* inputBuffer, UniLib::lib::BufferedNetworkPacket* outputBuffer)
	: Connection(url_host, port), Thread("bufConect"), mInputBuffer(inputBuffer), mOutputBuffer(outputBuffer)
{
}

BufferedConnection::~BufferedConnection() 
{
	mInputBuffer = NULL;
	mOutputBuffer = NULL;
}

int BufferedConnection::ThreadFunction()
{
	UniLib::EngineLog.writeToLog("run buffered connection thread");
	return 0;
}
