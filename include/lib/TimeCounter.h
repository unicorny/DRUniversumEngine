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
* @Date 26.11.17
*
* @Desc Class for measure time for profiling
*       Using QueryPerformanceCounter with windows
*/

#ifndef __DR_UNIVERSUM_LIB_TIME_COUNTER__
#define __DR_UNIVERSUM_LIB_TIME_COUNTER__

#include "UniversumLib.h"

namespace UniLib {
	namespace lib {
		class UNIVERSUM_LIB_API TimeCounter
		{
		public:
			TimeCounter() { restart(); }
			__inline__ void restart() {
#ifdef _WINDOWS_
				QueryPerformanceCounter(&mCounter);
#else
				mCounter = SDL_GetTicks();
#endif
			}
			double millis() const;
			double micros() const;
			std::string string() const;
		protected:
#ifdef _WINDOWS_
			LARGE_INTEGER mCounter;
#else 
			Uint32 mCounter;
#endif //_WINDOWS_
		};
	}
}

#endif // __DR_UNIVERSUM_LIB_TIME_COUNTER__