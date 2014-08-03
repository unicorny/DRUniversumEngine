#include "UniversumLib.h"

namespace UniLib {
	namespace lib {

		BufferedNetworkPacket::BufferedNetworkPacket()
		: mMutex(NULL), mExternDLLMutex(NULL), mExternDLLState(NOT_EXIST), mMutexLockState(false), exit(false)
		{
			mMutex = SDL_CreateMutex();
			if(!mMutex) LOG_WARNING_SDL();
		}

		DRReturn BufferedNetworkPacket::initExternDLLMutex()
		{
			mExternDLLMutex = SDL_CreateMutex();
			if(!mExternDLLMutex) LOG_ERROR_SDL(DR_ERROR);
			mExternDLLState = EXIST;
			return DR_OK;
		}

		void BufferedNetworkPacket::removeExternDLLMutex()
		{
			if(mExternDLLMutex) {
				mExternDLLState = DELETING;
				SDL_LockMutex(mExternDLLMutex);
				SDL_UnlockMutex(mExternDLLMutex);
				SDL_DestroyMutex(mExternDLLMutex);
				mExternDLLMutex = NULL;
				mExternDLLState = NOT_EXIST;
			}
		}

		BufferedNetworkPacket::~BufferedNetworkPacket()
		{
			exit = true;
			// must not be called, must be called from same instance how called initExternDLLMutex
			removeExternDLLMutex();
			if(mMutex) {
				SDL_LockMutex(mMutex);
				mMutexLockState = true;
				SDL_UnlockMutex(mMutex);
				mMutexLockState = false;
				SDL_DestroyMutex(mMutex);
				mMutex = NULL;
				
			}
		}
		void BufferedNetworkPacket::setURLAndHost(std::string url, std::string host)
		{
			mUrl = url;
			mHost = host;
		}
		BufferedNetworkPacket::LockingState BufferedNetworkPacket::lock(bool fromExternDLL/* = false*/)
		{
			if(exit) return ALREADY_LOCKED;
			if(!fromExternDLL) 
			{
				if(mExternDLLState == LOCKED) return ALREADY_LOCKED;
				if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(LOCKING_ERROR);
				mMutexLockState = true;
				return SUCCESSFULL;
			}
			else 
			{
				if(mExternDLLState == EXIST) 
				{
					if(mMutexLockState) return ALREADY_LOCKED;
					if(SDL_LockMutex(mExternDLLMutex)) LOG_ERROR_SDL(LOCKING_ERROR);
					mExternDLLState = LOCKED;
					return SUCCESSFULL;
				}
				else if(mExternDLLState == NOT_EXIST) {
					LOG_ERROR("extern dll mutex not initalized", LOCKING_ERROR);
				}
				else if(mExternDLLState == DELETING) {
					LOG_ERROR("extern dll mutex is about to vanish", LOCKING_ERROR);
				}
			}
			return LOCKING_ERROR;
		}

		BufferedNetworkPacket::LockingState BufferedNetworkPacket::unlock(bool fromExternDLL/* = false*/)
		{
			if(exit) return ALREADY_LOCKED;
			if(!fromExternDLL) 
			{
				if(SDL_UnlockMutex(mMutex)) LOG_ERROR_SDL(LOCKING_ERROR);
				mMutexLockState = false;
				return SUCCESSFULL;
			}
			else 
			{
				if(mExternDLLState == LOCKED) 
				{
					if(SDL_UnlockMutex(mExternDLLMutex)) LOG_ERROR_SDL(LOCKING_ERROR);
					mExternDLLState = EXIST;
					return SUCCESSFULL;
				}
				else if(mExternDLLState == NOT_EXIST) {
					LOG_ERROR("extern dll mutex not initalized", LOCKING_ERROR);
				} 
				else if(mExternDLLState == DELETING) {
					LOG_ERROR("extern dll mutex is about to vanish", LOCKING_ERROR);
				}

			}
			return LOCKING_ERROR;
		}

		void BufferedNetworkPacket::pushData(void* data, size_t size, bool fromExternDLL/* = false*/)
		{
			if(lock(fromExternDLL) == SUCCESSFULL) 
			{
				mBufferQueue.push(new DataPacket(data, size));
				unlock(fromExternDLL);
			}
		}

		void BufferedNetworkPacket::pushData(std::string string, bool fromExternDLL/* = false*/) 
		{
			pushData((void*)(string.data()), string.size(), fromExternDLL);
		}
		void BufferedNetworkPacket::pushData(Json::Value value, bool fromExternDLL/* = false*/)
		{
			std::string string;
			Json::Writer* writer;
#ifdef _UNI_LIB_DEBUG
			writer = new Json::StyledWriter();		
#else
			writer = new Json::FastWriter();
#endif
			pushData(writer->write(value), fromExternDLL);
			DR_SAVE_DELETE(writer);
		}
		
		std::string BufferedNetworkPacket::popDataString(bool fromExternDLL/* = false*/)
		{
			if(mBufferQueue.size() == 0) return "";
			if(lock(fromExternDLL) == SUCCESSFULL) 
			{			
				DataPacket* data = mBufferQueue.front();
				std::string dataString(static_cast<char*>(data->data), data->size);
				DR_SAVE_DELETE(data);			
				mBufferQueue.pop();
				unlock(fromExternDLL);
				return dataString;
			}
			return "";
		}

		Json::Value BufferedNetworkPacket::popData(bool fromExternDLL/* = false*/)
		{
			Json::Value returnValue;
			
			Json::Reader jsonReader;
			jsonReader.parse(popDataString(fromExternDLL), returnValue, false);			
			
			return returnValue;
		}

	}
}