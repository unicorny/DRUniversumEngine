#include "UniNetworkHeader.h"

using namespace Json;
using namespace Poco;
using namespace Poco::Net;

HTTPConnection::HTTPConnection(Json::Value cfg)
{
	mClientSession.setPort(cfg.get("port", Value(80)).asInt());
	mClientSession.setHost(cfg.get("ip", Value("localhost")).asString());
}

HTTPConnection::~HTTPConnection()
{

}

DRReturn HTTPConnection::run() 
{
	
}