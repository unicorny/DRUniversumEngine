#include "lib/Structures.h"

// *********************************************************
// DRNetServerConfig methods
// *********************************************************

void DRNetServerConfig::readFromJson(Json::Value value)
{
	url = value.get("url", "localhost").asString();
	port = value.get("port", 80).asInt();
	route = value.get("route", "").asString();
	serverType = getServerTypeFromString(value.get("type", "none").asString());
}

const char* DRNetServerConfig::getServerTypeString(DRNet_Server_Type type)
{
	switch(type) {
	case NET_SERVER_TYPE_HTTP: return "http";
	case NET_SERVER_TYPE_SOCKET: return "socket";
	default: return "none";
	}
}
DRNet_Server_Type DRNetServerConfig::getServerTypeFromString(std::string typeString)
{
	if(typeString == "http") return NET_SERVER_TYPE_HTTP;
	if(typeString == "socket") return NET_SERVER_TYPE_SOCKET;
	return NET_SERVER_TYPE_NONE;
}


// *********************************************************************
// DRNetRequest methods
// *********************************************************************

DRNetRequest::DRNetRequest()
	: url("localhost"), method(NET_REQUEST_METHOD_GET), contentType(NET_REQUEST_CONTENT_TYPE_NONE), userAgent("UniLib"), content(Json::objectValue) 
{

}
DRNet_Request_Method DRNetRequest::getRequestMethodFromString(std::string requestMethodString)
{
	if(requestMethodString == "POST" || requestMethodString == "post") {
		return NET_REQUEST_METHOD_POST;
	}
	if(requestMethodString == "GET" || requestMethodString == "get") {
		return NET_REQUEST_METHOD_GET;
	}
	return NET_REQUEST_METHOD_NONE;
}
const char* DRNetRequest::getRequestMethodAsString(DRNet_Request_Method requestMethod)
{
	switch(requestMethod) {
	case NET_REQUEST_METHOD_GET: return "GET";
	case NET_REQUEST_METHOD_POST: return "POST";
	default: return "none";
	}
}

DRNet_Request_ContentType DRNetRequest::getRequestContentTypeFromString(std::string contentTypeString)
{
	if(contentTypeString == "application/x-www-form-urlencoded") {
		return NET_REQUEST_CONTENT_TYPE_FORM_URLENCODED;
	}
	return NET_REQUEST_CONTENT_TYPE_NONE;
}
const char* DRNetRequest::getRequestContentTypeAsString(DRNet_Request_ContentType contentType)
{
	switch(contentType) {
	case NET_REQUEST_CONTENT_TYPE_FORM_URLENCODED: return "application/x-www-form-urlencoded";
	default: return "none";
	}
}