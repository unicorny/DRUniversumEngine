#include "UniNetworkHeader.h"

using namespace Json;
using namespace Poco;
using namespace Poco::Net;

HTTPConnection::HTTPConnection(const DRNetServerConfig& cfg)
	: Connection(1000)
{
	mClientSession.setPort(cfg.port);
	mClientSession.setHost(cfg.ip);
}

HTTPConnection::~HTTPConnection()
{

}


DRReturn HTTPConnection::run() 
{
	// get send request from queue
	mRequestMutex.lock();
	if(mSendRequests.size() == 0) {
		mRequestMutex.unlock();
		return DR_OK;
	}
//	assert(mSendRequests.size());
	std::string requestString = mSendRequests.front();
	Json::Value sendRequest;
	parseJson(requestString, sendRequest);
	mSendRequests.pop();
	mRequestMutex.unlock();

	try {
		// parse and request
		HTTPRequest request(sendRequest.get("method", "GET").asString(), sendRequest.get("url", "").asString(), "HTTP/1.1");

		request.set("User-Agent", sendRequest.get("userAgent", "UniLib").asString());
		//printf("user agent: %s\n",  sendRequest.get("userAgent", "UniLib").asString().data());
		//POCO_LOG_WARNING(std::string("user agent: ") + sendRequest.get("userAgent", "UniLib").asString());
		if(sendRequest.get("contentType", "").asString() != "") {
			request.setContentType(sendRequest.get("contentType", "").asString());
			std::string body = sendRequest.get("content", "").asString();
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
		mRecvMutex.lock();
		mReciveDatas.push(ostr.str());
		mRecvMutex.unlock();
	} catch (Poco::Exception what) {
		POCO_LOG_FATAL(std::string("error by reciving: ") + std::string(what.displayText()));
		return DR_ERROR;
	}

	return DR_OK;	
}

DRNet_Status HTTPConnection::send(std::string sendRequest)
{
	mRequestMutex.lock();
	mSendRequests.push(sendRequest);
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