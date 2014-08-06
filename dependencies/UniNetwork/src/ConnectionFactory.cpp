#include "UniNetworkHeader.h"

using namespace Poco;

ConnectionFactory::ConnectionFactory(std::string logFilename/* = std::string("log/connectionFactoryLog.txt")*/)
	: mLogFilename(logFilename)
{
}

ConnectionFactory::~ConnectionFactory() 
{
}

void ConnectionFactory::logFatal(std::string msg, std::string functionName)
{
	AutoPtr<FileChannel> logging = new FileChannel(mLogFilename);
	logging->open();
	Poco::Message p_msg(std::string("[Connection::") + functionName + std::string("]"),
				  msg,
				  Poco::Message::PRIO_FATAL);
	logging->log(p_msg);
	logging->close();
}


Connection* ConnectionFactory::createConnection(std::string connectionConfig, std::string section)
{
	Json::Reader reader;
	Json::Value value;
	reader.parse(connectionConfig, value);
	if(value.empty()) 
	{
		logFatal(std::string("parsing error: ") + std::string(reader.getFormattedErrorMessages()), std::string(__FUNCTION__));
		poco_debugger_msg("parsing error");
		return NULL;		
	}
	if(value.type() == Json::objectValue) {
		Json::Value sectionValue = value.get(section, Json::Value());
		if(!sectionValue.empty()) {
			Json::Value typeValue = sectionValue.get("type", Json::Value("http"));
			if(typeValue.asString() == std::string("http")) {
				return new HTTPConnection(sectionValue);
			}
		}
		else {
			logFatal(std::string("section: ") + section + std::string(" doesn't exist in json"), std::string(__FUNCTION__));
			poco_debugger_msg("section not found error");
			return NULL;
		}
	}
	return NULL;
}