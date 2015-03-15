#include "ConnectionFactory.h"
#include "HTTPConnection.h"

using namespace Poco;

ConnectionFactory::ConnectionFactory(std::string logFilename/* = std::string("log/connectionFactoryLog.txt")*/)
	: Loggable(logFilename, "ConnectionFactory")
{
}

ConnectionFactory::~ConnectionFactory() 
{
}




Connection* ConnectionFactory::createConnection(const DRNetServerConfig& cfg)
{
	switch(cfg.serverType) {
	case NET_SERVER_TYPE_HTTP: return new HTTPConnection(cfg);
	}
	return NULL;
}