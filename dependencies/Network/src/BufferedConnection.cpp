#include "BufferedConnection.h"

BufferedConnection::BufferedConnection(std::string name, Timer* timer, const char* url_host, int port,
									   BufferedNetworkPacket* inputBuffer, BufferedNetworkPacket* outputBuffer)
	: Connection(url_host, port), TimingThread(name, BUFFERED_CONNECTION_RERUN_DELAY_MILLISECONDS, timer, "bufConect"),
	  mInputBuffer(inputBuffer), mOutputBuffer(outputBuffer)
{
	inputBuffer->setURLAndHost(mURL, mHOST);
}

BufferedConnection::~BufferedConnection() 
{
	mInputBuffer = NULL;
	mOutputBuffer = NULL;
}

int BufferedConnection::ThreadFunction()
{
	//UniLib::EngineLog.writeToLog("run buffered connection thread");
	TCPsocket socket = getSocket();
    if(!socket) LOG_ERROR("socket error", -1);

	// is there somthing for us to recive?
	if(recv()) LOG_ERROR("error by reciving datas", -2);
	// see if we have something to send

	if(send()) LOG_ERROR("error by sending datas", -3);

	// wait on answear
	if(recv()) LOG_ERROR("error by waiting on reciving datas", -4);

	return 0;
}

DRReturn BufferedConnection::send()
{
	std::string in =  mInputBuffer->popDataString();
	if(in.length() > 0) {
		if(in.length() > BUFFERED_CONNECTION_MAX_RECIVE_DATA_BLOCK_BYTES) 
			LOG_ERROR("to many data so send", DR_ERROR);
		int sendedBytes = SDLNet_TCP_Send(getSocket(), in.data(), in.length());
		if(static_cast<unsigned int>(sendedBytes) < in.length()) LOG_ERROR("data couldn't be successfully sended", DR_ERROR);
	}
	return DR_OK;
}

DRReturn BufferedConnection::recv()
{
	RecivingBuffer* buffer = new RecivingBuffer;
	int recivedBytes = 0;
	int sumRecivedBytes = 0;
	std::list<RecivingBuffer*> bufferList;
	
	while(recivedBytes = SDLNet_TCP_Recv(getSocket(), &buffer->buffer[buffer->readedBytes], buffer->emptyBytes))
	{
		sumRecivedBytes += recivedBytes;
		if(buffer->recived(recivedBytes) == 0)
		{
			bufferList.push_back(buffer);
			if(bufferList.size() >= BUFFERED_CONNECTION_MAX_RECIVE_DATA_BLOCK_BYTES / BUFFERED_CONNECTION_RECIVE_BUFFER_SIZE_BYTES)
				LOG_ERROR("recived Data is way to much, cancel connection, attention data leak!!!", DR_ERROR);
			buffer = new RecivingBuffer;
		}
	}
	if(buffer->readedBytes > 0) 
		bufferList.push_back(buffer);

	char* resultBuffer = new char[sumRecivedBytes+1];
	memset(resultBuffer,0, sumRecivedBytes+1);
	int cursor = 0;
	for(std::list<RecivingBuffer*>::iterator it = bufferList.begin(); it != bufferList.end(); it++)
	{
		if(cursor + (*it)->readedBytes > sumRecivedBytes) LOG_ERROR("buffer overflow, please check code", DR_ERROR);
		memcpy(&resultBuffer[cursor], (*it)->buffer, (*it)->readedBytes);
		cursor += (*it)->readedBytes;
		DR_SAVE_DELETE(*it);
		it = bufferList.erase(it);
	}
	mOutputBuffer->pushData(resultBuffer, sumRecivedBytes);
	return DR_OK;

}