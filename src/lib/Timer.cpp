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

		DRReturn Timer::addTimer(DRResourcePtr<TimerCallback> callbackObject, Uint32 timeIntervall, int loopCount/* = -1*/)
		{
			if(exit) return DR_NOT_ERROR;
			if(!mMutex) LOG_ERROR("no mutex to lock available", DR_ERROR);
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(DR_ERROR);

			mRegisteredAtTimer.insert(TIMER_TIMER_ENTRY(SDL_GetTicks() + timeIntervall,  TimerEntry(callbackObject, timeIntervall, loopCount)));

			SDL_UnlockMutex(mMutex);
			return DR_OK;
		}

		DRReturn Timer::move(float timeSinceLastFrame) 
		{
			if(exit) return DR_NOT_ERROR;
			if(!mMutex) LOG_ERROR("no mutex to lock available", DR_ERROR);
			if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(DR_ERROR);

			std::map<Uint32, TimerEntry>::iterator it = mRegisteredAtTimer.begin();

			if(it->first <= SDL_GetTicks() ) {
				it->second.callback->callFromTimer();
				if(it->second.nextLoop()) {
					mRegisteredAtTimer.insert(TIMER_TIMER_ENTRY(SDL_GetTicks() + it->second.timeIntervall,  it->second));
				}
				mRegisteredAtTimer.erase(it);
			}

			SDL_UnlockMutex(mMutex);
			return DR_OK;
		}
	}
}