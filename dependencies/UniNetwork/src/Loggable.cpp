#include "UniNetworkHeader.h"

using namespace Poco;


Loggable::Loggable(std::string logFilename, std::string className)
	: mLogFilename(logFilename), mClassName(className)
{
}

Loggable::~Loggable()
{
}

void Loggable::logFatal(std::string msg, std::string functionName)
{
	log(msg, functionName, Poco::Message::PRIO_FATAL);
}

void Loggable::logWarning(std::string msg, std::string functionName)
{
	log(msg, functionName, Poco::Message::PRIO_WARNING);
}
void Loggable::logError(std::string msg, std::string functionName)
{
	log(msg, functionName, Poco::Message::PRIO_ERROR);
}

void Loggable::log(std::string msg, std::string functionName, Poco::Message::Priority prio)
{
	AutoPtr<FileChannel> logging = new FileChannel(mLogFilename);
	logging->open();
	Poco::Message p_msg(std::string("[Connection::") + functionName + std::string("]"),
				  msg,
				  prio);
	logging->log(p_msg);
	logging->close();
}