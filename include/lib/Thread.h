/*/*************************************************************************
*                                                                         *
* UniversumLib, collection of classes for generating and go through a     *
* whole universe. It is for my Gameproject Spacecraft					   *
* Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
* Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
*                                                                         *
* This program is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* any later version.													   *
*																		   *
* This program is distributed in the hope that it will be useful,	       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
* GNU General Public License for more details.							   *
*																		   *
* You should have received a copy of the GNU General Public License	   *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                         *
***************************************************************************/

/**
 * @Author Dario Rekowski
 * 
 * @Date 13.08.12
 * 
 * @Desc Class for easy handling threading
 */
 
#ifndef __DR_UNIVERSUM_LIB_THREAD__
#define __DR_UNIVERSUM_LIB_THREAD__

//#include "Timer.h"
#include <sdl/SDL_thread.h>

namespace UniLib {
    namespace lib {
        class UNIVERSUM_LIB_API Thread
        {
        public:
            //! \param threadName used since SDL 1.3, for BeOS max. 32, for Linux max 16, for Visual Studio 6.0 max 9 char
			//! \param createInConstructor set to false if thread shouldn't create in constructor, for example if SDL isn't loaded yet
            Thread(const char* threadName = NULL, bool createInConstructor = true);
            virtual ~Thread();

            __inline__ DRReturn threadLock() {if(SDL_LockMutex(mutex)) LOG_ERROR_SDL(DR_ERROR); return DR_OK;}
            __inline__ DRReturn threadUnlock() {if(SDL_UnlockMutex(mutex)) LOG_ERROR_SDL(DR_ERROR); return DR_OK;} 
            // signal data chance, will continue thread, if he is paused
            DRReturn condSignal();

			//! \param threadName used since SDL 1.3, for BeOS max. 32, for Linux max 16, for Visual Studio 6.0 max 9 char
			DRReturn init(const char* threadName);
        protected:
            //! \brief will be called every time from thread, when condSignal was called
            //! will be called from thread with locked working mutex,<br>
            //! mutex will be unlock after calling this function
            //! \return if return isn't 0, thread will exit
            virtual int ThreadFunction() = 0;

            static int run(void* data);

            SDL_mutex*		   mutex;
            SDL_Thread*		   thread;
            SDL_cond*		   condition;
            SDL_sem*		   semaphore;
            bool               exitCalled;
        };

		class UNIVERSUM_LIB_API TimingThread: public Thread, TimerCallback 
		{
			public:
				TimingThread(std::string name, Uint32 rerunDelay, Timer* timerOnWhichToAttach, const char* threadName = NULL);
				~TimingThread();
				virtual TimerReturn callFromTimer();
				virtual int ThreadFunction() = 0;

				__inline__ std::string getName() {return mName;}

			protected:
				std::string mName;

			private:
				Timer* mTimer;
				DRResourcePtr<TimerCallback> myself;

		};

    }
}


#endif //__DR_UNIVERSUM_LIB_THREAD__
