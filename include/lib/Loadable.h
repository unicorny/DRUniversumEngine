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

/*!

\brief simple class to provide loading state

\author Dario Rekowski

\date 17.04.2016
*/

#ifndef DR_UNIVERSUM_LIB_LIB_LOADABLE_H
#define DR_UNIVERSUM_LIB_LIB_LOADABLE_H

#include "UniversumLib.h"

namespace UniLib {
	namespace lib {
		class UNIVERSUM_LIB_API Loadable 
		{
		public:
			Loadable():  mLoadingState(LOADING_STATE_EMPTY) {};

			__inline__ LoadingState checkLoadingState() { return mLoadingState; }
		protected:
			void setLoadingState(LoadingState state) { mLoadingState = state; }
			LoadingState mLoadingState;
		};
	}
}

#endif // !DR_UNIVERSUM_LIB_LIB_LOADABLE_H
