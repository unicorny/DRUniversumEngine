#include "UniversumLib.h"

namespace UniLib {
	namespace lib {

		BufferedNetworkPacket::BufferedNetworkPacket()
		: mMutex(NULL)
		{
			mMutex = SDL_CreateMutex();
			if(!mMutex) LOG_WARNING_SDL();
		}

		BufferedNetworkPacket::~BufferedNetworkPacket()
		{
			if(mMutex) {
				SDL_LockMutex(mMutex);
				SDL_UnlockMutex(mMutex);
				SDL_DestroyMutex(mMutex);
				mMutex = NULL;
			}
		}

		void BufferedNetworkPacket::pushData(void* data, size_t size)
		{
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

		Json::Value BufferedNetworkPacket::popData()
		{
			Json::Value returnValue;
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(returnValue);
			if(mBufferQueue.size() == 0) return returnValue;
			DataPacket* data = mBufferQueue.front();
			std::string dataString(static_cast<char*>(data->data), data->size);
			Json::Reader jsonReader;
			jsonReader.parse(dataString, returnValue, false);			

			DR_SAVE_DELETE(data);
			
			mBufferQueue.pop();

			SDL_UnlockMutex(mMutex);
			return returnValue;
		}

	}
}