#include "HTTPConnection.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include <sstream>

using namespace Json;
using namespace Poco;
using namespace Poco::Net;

HTTPConnection::HTTPConnection(const DRNetServerConfig& cfg)
	: Connection(1000)
{
	mClientSession.setPort((Poco::UInt16)cfg.port);
	mClientSession.setHost(cfg.url);
}

HTTPConnection::~HTTPConnection()
{

}


DRReturn HTTPConnection::run() 
{
	// get send request from queue
	mRequestMutex.lock();
	if(mSendRequests.size() == 0 && mSendCommandRequests.size() == 0) {
		mRequestMutex.unlock();
		return DR_OK;
	}
	DRNetRequest requestCfg;
	UniLib::server::RequestCommand* command = NULL;
	if(mSendRequests.size()) {
		requestCfg = mSendRequests.front();
		mSendRequests.pop();
	} else {
		requestCfg = mSendCommandRequests.front().request;
		command = mSendCommandRequests.front().command;
		mSendCommandRequests.pop();
	}
	mRequestMutex.unlock();

	try {
		// parse and request
		HTTPRequest request(DRNetRequest::getRequestMethodAsString(requestCfg.method),
							requestCfg.url, "HTTP/1.1");

		request.set("User-Agent", requestCfg.userAgent);
		//printf("user agent: %s\n",  sendRequest.get("userAgent", "UniLib").asString().data());
		//POCO_LOG_WARNING(std::string("user agent: ") + sendRequest.get("userAgent", "UniLib").asString());
		if(requestCfg.contentType != NET_REQUEST_CONTENT_TYPE_NONE) {
			request.setContentType(DRNetRequest::getRequestContentTypeAsString(requestCfg.contentType));
			Json::FastWriter writer;
			std::string body = "json=" + writer.write(requestCfg.content); //requestCfg.content.asString();
			//body.append("\r\n0\r\n");
			request.setChunkedTransferEncoding(false);
			request.setContentLength((int)body.length());
		
			mClientSession.sendRequest(request) << body;		
		}
		else {
			mClientSession.sendRequest(request);
		}
	} catch(Poco::Exception what) {
		POCO_LOG_FATAL(std::string("error by sending: ") + std::string(what.displayText()));
		return DR_ERROR;
	}

	try {
		// recv answear
		HTTPResponse response;
	
		std::istream& is = mClientSession.receiveResponse(response);// >> responseString;
		std::ostringstream ostr;	
		StreamCopier::copyStream(is, ostr);	
		if(command) {
			std::string outStr = ostr.str(); 
			//printf(outStr.data());
			command->execute(NET_COMPLETE, outStr);
		} else {
			mRecvMutex.lock();
			mReciveDatas.push(ostr.str());
			mRecvMutex.unlock();
		}
	} catch (Poco::Exception what) {
		POCO_LOG_FATAL(std::string("error by reciving: ") + std::string(what.displayText()));
		return DR_ERROR;
	}

	return DR_OK;	
}

DRNet_Status HTTPConnection::send(const DRNetRequest& sendRequest)
{
	mRequestMutex.lock();
	mSendRequests.push(sendRequest);
	mRequestMutex.unlock();
	return NET_OK;
}

DRNet_Status HTTPConnection::send(const DRNetRequest& sendRequest, UniLib::server::RequestCommand* command)
{
	mRequestMutex.lock();
	mSendCommandRequests.push(RequestWithReturnCommand(sendRequest, command));
	mRequestMutex.unlock();
	return NET_OK;
}

DRNet_Status HTTPConnection::recv(std::string& recvDatas)
{
	mRecvMutex.lock();
	if(mReciveDatas.size() == 0) {
		mRecvMutex.unlock();
		return NET_NOT_READY;
	}
	recvDatas = mReciveDatas.front();
	mReciveDatas.pop();
	mRecvMutex.unlock();
	return NET_COMPLETE;	
}