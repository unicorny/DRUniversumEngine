#include "UniNetworkHeader.h"

using namespace Poco;

Connection::Connection(long periodicRunningInterval)
	: mTimer(0, periodicRunningInterval)
{
	TimerCallback<Connection> tc(*this, &Connection::resume);
	mTimer.start(tc);
}

Connection::~Connection()
{
	mThreadRunningMutex.lock();
	mThreadRunningMutex.unlock();
	mTimer.restart(0);
}

// called from timer
void Connection::resume(Timer& timer) 
{
	mThreadRunningMutex.lock();
	// error in runImpl, then exit timer
	if(run()) {
		mTimer.restart(0);
	}
	mThreadRunningMutex.unlock();
}

