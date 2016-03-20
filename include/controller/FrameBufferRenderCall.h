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
* \date: 17.03.16
*
* \desc: go through RenderToTexture tasks and call them
*/

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_FRAME_BUFFER_RENDER_CALL_H
#define __DR_UNIVERSUM_LIB_CONTROLLER_FRAME_BUFFER_RENDER_CALL_H
#include "GPUScheduler.h"
#include "lib/MultithreadContainer.h"
namespace UniLib {
	namespace generator {
		class RenderToTexture;
	}
	namespace controller {
		class UNIVERSUM_LIB_API FrameBufferRenderCall : public GPURenderCall, protected lib::MultithreadContainer
		{
		public:
			FrameBufferRenderCall();
			virtual ~FrameBufferRenderCall();

			virtual DRReturn render(float timeSinceLastFrame);
			// if render return not DR_OK, Call will be removed from List and kicked will be called
			virtual void kicked();
			// will be called if render call need to much time
			// \param percent used up percent time of render main loop
			virtual void youNeedToLong(float percent);

			DRReturn addRenderToTextureTask(generator::RenderToTexture* task, bool fastTask = true);

		protected:
			std::deque<generator::RenderToTexture*> mFastRenderToTextureTasks;

		};
	}
}

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_FRAME_BUFFER_RENDER_CALL_H