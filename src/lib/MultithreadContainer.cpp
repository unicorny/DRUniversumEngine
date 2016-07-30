//#include "lib/MultithreadContainer.h"
#include "UniversumLib.h"

namespace UniLib {
	namespace lib {
		MultithreadContainer::MultithreadContainer() : mWorkMutex(SDL_CreateMutex())
		{
			if(!mWorkMutex) LOG_WARNING_SDL();
		}

		MultithreadContainer::~MultithreadContainer()
		{
			SDL_DestroyMutex(mWorkMutex);
		}
	}
}