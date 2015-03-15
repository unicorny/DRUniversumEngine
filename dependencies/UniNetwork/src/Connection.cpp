#include "Connection.h"

using namespace Poco;

Connection::Connection(long periodicRunningInterval)
	: Loggable("connection.log", "Connection"), mTimer(1000, periodicRunningInterval)
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

DRReturn Connection::parseJson(std::string jsonString, Json::Value& json)
{	
	Json::Reader reader;
	reader.parse(jsonString, json);
	if(json.empty()) 
	{
		POCO_LOG_FATAL(std::string("parsing error: ") + std::string(reader.getFormattedErrorMessages()));
		//poco_debugger_msg("parsing error");
		return DR_ERROR;		
	}
	return DR_OK;
}