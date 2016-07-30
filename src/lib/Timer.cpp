 //#include "lib/Timer.h"
#include "UniversumLib.h"

namespace UniLib {
	namespace lib {

		Timer::Timer()
			: exit(false)
		{
			mMutex = SDL_CreateMutex();
			if(!mMutex) LOG_ERROR_SDL_VOID();
		}

		Timer::~Timer() 
		{
			exit = true;
			if(mMutex) {
				SDL_LockMutex(mMutex);
				SDL_UnlockMutex(mMutex);
			}
			mRegisteredAtTimer.clear();
		}

		// -----------------------------------------------------------------------------------------------------

		DRReturn Timer::addTimer(std::string name, DRResourcePtr<TimerCallback> callbackObject, Uint32 timeIntervall, int loopCount/* = -1*/)
		{
			if(exit) return DR_NOT_ERROR;
			if(!mMutex) LOG_ERROR("no mutex to lock available", DR_ERROR);
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(DR_ERROR);

			mRegisteredAtTimer.insert(TIMER_TIMER_ENTRY(SDL_GetTicks() + timeIntervall,  TimerEntry(callbackObject, timeIntervall, loopCount, name)));

			SDL_UnlockMutex(mMutex);
			return DR_OK;
		}

		int Timer::removeTimer(std::string name)
		{
			if(exit) return -1;
			if(!mMutex) LOG_ERROR("no mutex to lock available", -1);
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(-1);

			size_t eraseCount = 0;
			bool somethingErased = false;
			do {
				somethingErased = false;
				for(std::multimap<Uint32, TimerEntry>::iterator it = mRegisteredAtTimer.begin(); it != mRegisteredAtTimer.end(); it++)
				{
					if(name == it->second.name)
					{
						mRegisteredAtTimer.erase(it);
						eraseCount++;
						somethingErased = true;
						break;
					}
				}
			} while(somethingErased);

			SDL_UnlockMutex(mMutex);

			return eraseCount;
		}

		DRReturn Timer::move(float timeSinceLastFrame) 
		{
			if(exit) return DR_NOT_ERROR;
			if(!mMutex) LOG_ERROR("no mutex to lock available", DR_ERROR);
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(DR_ERROR);

			std::multimap<Uint32, TimerEntry>::iterator it = mRegisteredAtTimer.begin();

			if(it->first <= SDL_GetTicks() ) {
				TimerReturn ret = it->second.callback->callFromTimer();
				if(it->second.nextLoop() && ret == GO_ON) {
					 mRegisteredAtTimer.insert(TIMER_TIMER_ENTRY(SDL_GetTicks() + it->second.timeIntervall,  it->second));
				}
				mRegisteredAtTimer.erase(it);
				if(ret == REPORT_ERROR) {
					SDL_UnlockMutex(mMutex);
					LOG_ERROR("report error from timer callback", DR_ERROR);
				}
				
			}

			SDL_UnlockMutex(mMutex);
			return DR_OK;
		}
	}
}