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
//#include "UniversumLib.h"

#ifndef __UNIVERSUM_LIB_LIB_TIMER__
#define __UNIVERSUM_LIB_LIB_TIMER__

namespace UniLib {
	namespace lib {

		enum TimerReturn {
			GO_ON = 0,
			REMOVE_ME = 1,
			REPORT_ERROR = 2
		};
#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
		enum UNIVERSUM_LIB_API TimerReturn;
#endif

		class UNIVERSUM_LIB_API TimerCallback : public DRIResource
		{
		public:
			virtual TimerReturn callFromTimer() = 0;
			virtual const char* getResourceType() const {return "TimerCallback";};
			virtual bool less_than(DRIResource& b) const {return false;};		
		};

		class UNIVERSUM_LIB_API Timer  
		{
		public: 
			Timer();
			~Timer();

			/*!
				add timer callback object
				\param timeIntervall intervall in milliseconds 
				\param loopCount 0 for one call, -1 for loop
			*/
			DRReturn addTimer(std::string name, DRResourcePtr<TimerCallback> callbackObject, Uint32 timeIntervall, int loopCount = -1);

			/*!
				\brief remove all timer with name
				function is not really fast, because all appended timerCallback will be checked 

				\return removed timer, or -1 by error
			*/
			int removeTimer(std::string name);

			/*
				\brief update timer map, maybe call timer... (only one per frame)
			*/
			DRReturn move(float timeSinceLastFrame);
		private:
			struct TimerEntry {
				// functions
				TimerEntry(DRResourcePtr<TimerCallback> _callback, Uint32 _timeIntervall, int _loopCount, std::string _name)
					: callback(_callback), timeIntervall(_timeIntervall), initLoopCount(_loopCount), currentLoopCount(0), name(_name) {}
				~TimerEntry()  {}
				// \return true if we can run once again
				bool nextLoop() {
					currentLoopCount++;
					if(initLoopCount < 0 || initLoopCount-1 > currentLoopCount) return true;
					return false;
				}

				// variables
				DRResourcePtr<TimerCallback> callback;
				Uint32 timeIntervall;
				int initLoopCount;
				int currentLoopCount;
				std::string name;
			};
			// int key = time since programm start to run
			std::multimap<Uint32, TimerEntry> mRegisteredAtTimer;
			typedef std::pair<Uint32, TimerEntry> TIMER_TIMER_ENTRY;
			SDL_mutex*	mMutex;
			bool		exit;

		};
	}
}

#endif //__UNIVERSUM_LIB_LIB_TIMER__