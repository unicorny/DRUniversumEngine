#include "UniversumLib.h"

namespace UniLib {
	namespace lib {

		BufferedNetworkPacket::BufferedNetworkPacket()
		: mMutex(NULL), exit(false)
		{
			mMutex = SDL_CreateMutex();
			if(!mMutex) LOG_WARNING_SDL();
		}

		BufferedNetworkPacket::~BufferedNetworkPacket()
		{
			exit = true;
			if(mMutex) {
				SDL_LockMutex(mMutex);
				SDL_UnlockMutex(mMutex);
				SDL_DestroyMutex(mMutex);
				mMutex = NULL;
			}
		}
		void BufferedNetworkPacket::setURLAndHost(std::string url, std::string host)
		{
			mUrl = url;
			mHost = host;
		}

		void BufferedNetworkPacket::pushData(void* data, size_t size)
		{
			if(exit) return;
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL_VOID();
			mBufferQueue.push(new DataPacket(data, size));
			SDL_UnlockMutex(mMutex);
		}

		void BufferedNetworkPacket::pushData(std::string string) 
		{
			pushData((void*)(string.data()), string.size());
		}
		void BufferedNetworkPacket::pushData(Json::Value value)
		{
			std::string string;
			Json::Writer* writer;
#ifdef _UNI_LIB_DEBUG
			writer = new Json::StyledWriter();		
#else
			writer = new Json::FastWriter();
#endif
			pushData(writer->write(value));
			DR_SAVE_DELETE(writer);
		}
		DRReturn BufferedNetworkPacket::pushDataWrapHTTPRequest(Json::Value json, std::string userAgent, std::string parameter, DRNet_RequestTyp requestType/* = NET_GET*/)
		{
			std::string string;
			Json::Writer* writer;
#ifdef _UNI_LIB_DEBUG
			writer = new Json::StyledWriter();		
#else
			writer = new Json::FastWriter();
#endif
			std::string payload = writer->write(json);
			std::string request;

			// typ, POST or GET
			if(requestType == NET_GET)         request = "GET ";
			else if(requestType == NET_POST)   request = "POST ";
			else LOG_ERROR("ungueltiger Typ", DR_ERROR);

			// url, adresse der genauen seite
			request += mUrl;

			{
			   parameter += "&" + payload;
			}

			// parameter anhängen bei GET
			if(requestType == NET_GET)
				request += "?" + parameter;
			//  protocol and version
			request += " HTTP/1.1\r\n";
			// user-agent (also der browser)
			request += "User-Agent: " + userAgent + "\r\n";
			// Host
			request += "Host: " + mHost + "\r\n";

			request += "Accept: */*\r\n";
			request += "Accept-Encoding: text/html\r\n";
			if(requestType == NET_POST)
			{
				char temp[256];
				sprintf(temp, "Content-Length: %d\r\n", parameter.length());
				request += std::string(temp);
				request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
				request += parameter + std::string("\r\n");
			}
			request += "\r\n";
			pushData(request);

			return DR_OK;
		}


		std::string BufferedNetworkPacket::popDataString()
		{
			if(exit) return "";
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL("");
			if(mBufferQueue.size() == 0) return "";
			DataPacket* data = mBufferQueue.front();
			std::string dataString(static_cast<char*>(data->data), data->size);
			DR_SAVE_DELETE(data);			
			mBufferQueue.pop();
			SDL_UnlockMutex(mMutex);

			return dataString;
		}

		Json::Value BufferedNetworkPacket::popData()
		{
			Json::Value returnValue;
			
			Json::Reader jsonReader;
			jsonReader.parse(popDataString(), returnValue, false);			
			
			return returnValue;
		}

	}
}