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