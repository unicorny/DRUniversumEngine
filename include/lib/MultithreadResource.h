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
 *
 * \author: Dario Rekowski
 *
 * \date: 06.03.2016
 *
 * \desc: a resource with loading state and working mutex
 *
 *
 */

#ifndef __DR_UNIVERSUM_LIB_LIB_MULTITHREAD_RESOURCE_H
#define __DR_UNIVERSUM_LIB_LIB_MULTITHREAD_RESOURCE_H

#include "UniversumLib.h"

namespace UniLib {
	namespace lib {
		class UNIVERSUM_LIB_API MultithreadResource : public DRIResource, protected MultithreadContainer
		{
		public:
			MultithreadResource() : mLoadingState(LOADING_STATE_EMPTY) {};
			virtual ~MultithreadResource() {};

			virtual bool less_than(MultithreadResource& b) { return mLoadingState < b.mLoadingState; }
			
			// check and manipulate loading state
			__inline__ LoadingState checkLoadingState() { LoadingState state; lock(); state = mLoadingState; unlock(); return state; }
			
		protected:
			virtual void setLoadingState(LoadingState state) { lock(); mLoadingState = state; unlock(); }
			LoadingState mLoadingState;
		};
	}
}

#endif //__DR_UNIVERSUM_LIB_LIB_MULTITHREAD_RESOURCE_H