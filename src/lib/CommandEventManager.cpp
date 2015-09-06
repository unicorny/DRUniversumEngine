#include "lib/CommandEventManager.h"
#include "controller/Task.h"

namespace UniLib {
	namespace lib {
		CommandEventManager::CommandEventManager()
			: mWorkingMutex(NULL)
		{

		}

		CommandEventManager::~CommandEventManager()
		{

		}

		DRReturn CommandEventManager::init()
		{
			mWorkingMutex = SDL_CreateMutex();
			return DR_OK;
		}

		void CommandEventManager::exit()
		{
			SDL_LockMutex(mWorkingMutex);
			for(CommandMapIterator it1 = mCommandsMap.begin(); it1 != mCommandsMap.end(); it1++) {
				it1->second.clear();
			}
			mCommandsMap.clear();
			SDL_UnlockMutex(mWorkingMutex);
			SDL_DestroyMutex(mWorkingMutex);
			mWorkingMutex = NULL;
		}

		DRReturn CommandEventManager::addCommandForEvent(const char* eventName, controller::Command* command)
		{
			
#ifdef _DEBUG
			SDL_LockMutex(mWorkingMutex);
			// doubletten check
			HASH hash = DRMakeStringHash(eventName);
			HashNameMapIterator it = mHashNameMap.find(hash);
			if(it != mHashNameMap.end()) {
				assert(it->second == std::string(eventName));
			} else {
				mHashNameMap.insert(HashNameMapPair(hash, std::string(eventName)));
			}
			SDL_UnlockMutex(mWorkingMutex);
#endif
			return addCommandForEvent(DRMakeStringHash(eventName), command);
		}
		DRReturn CommandEventManager::addCommandForEvent(HASH eventHash, controller::Command* command)
		{
			SDL_LockMutex(mWorkingMutex);
			CommandMapIterator it = mCommandsMap.find(eventHash);
			if(it == mCommandsMap.end()) {
				mCommandsMap.insert(CommandMapPair(eventHash, CommandList()));
				it = mCommandsMap.find(eventHash);
			}
			if(it != mCommandsMap.end()) {
				it->second.push_back(command);
			} else {
				SDL_UnlockMutex(mWorkingMutex);
				LOG_ERROR("error adding command to map", DR_ERROR);
			}
			SDL_UnlockMutex(mWorkingMutex);
			return DR_OK;
		}
		void CommandEventManager::removeCommandForEvent(const char* eventName, controller::Command* command)
		{
			removeCommandForEvent(DRMakeStringHash(eventName), command);
		}
		void CommandEventManager::removeCommandForEvent(HASH eventHash, controller::Command* command)
		{
			SDL_LockMutex(mWorkingMutex);
			CommandMapIterator it = mCommandsMap.find(eventHash);
			if(it != mCommandsMap.end()) {
				for(CommandList::iterator commandIt = it->second.begin(); commandIt != it->second.end(); commandIt++) {
					if(*commandIt == command) {
						it->second.erase(commandIt);
						SDL_UnlockMutex(mWorkingMutex);
						return;
					}
				}
			}
			SDL_UnlockMutex(mWorkingMutex);
		}

		DRReturn CommandEventManager::triggerEvent(const char* name, controller::Task* task) 
		{
			return triggerEvent(DRMakeStringHash(name), task);
		}
		DRReturn CommandEventManager::triggerEvent(HASH eventHash, controller::Task* task)
		{
			SDL_LockMutex(mWorkingMutex);
			CommandMapIterator it = mCommandsMap.find(eventHash);
			DRReturn ret = DR_OK;
			if(it != mCommandsMap.end()) {
				for(CommandList::iterator commandIt = it->second.begin(); commandIt != it->second.end(); commandIt++) {
					if((*commandIt)->taskFinished(task)) {
						ret = DR_ERROR;
					}
				}
			}
			SDL_UnlockMutex(mWorkingMutex);
			return ret;
		}
	}
}