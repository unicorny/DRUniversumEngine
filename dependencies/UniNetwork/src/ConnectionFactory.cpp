#include "UniNetworkHeader.h"

using namespace Poco;

ConnectionFactory::ConnectionFactory(std::string logFilename/* = std::string("log/connectionFactoryLog.txt")*/)
	: Loggable(logFilename, "ConnectionFactory")
{
}

ConnectionFactory::~ConnectionFactory() 
{
}




Connection* ConnectionFactory::createConnection(std::string connectionConfig, std::string section)
{
	Json::Reader reader;
	Json::Value value;
	reader.parse(connectionConfig, value);
	if(value.empty()) 
	{
		POCO_LOG_FATAL(std::string("parsing error: ") + std::string(reader.getFormattedErrorMessages()));
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