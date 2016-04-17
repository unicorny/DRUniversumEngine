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
* \author: Dario Rekowski
*
* \date: 17.04.2016
*
* \desc: Interface for frame buffer
*/

#ifndef __UNIVERSUM_LIB_VIEW_FRAME_BUFFER_H
#define __UNIVERSUM_LIB_VIEW_FRAME_BUFFER_H

#include "UniversumLib.h"

namespace UniLib {
	namespace view {

		class Texture;
		typedef DRResourcePtr<Texture> TexturePtr;

		class UNIVERSUM_LIB_API FrameBuffer
		{
		public:
			FrameBuffer(view::TexturePtr texture) : mTexture(texture) {}
			FrameBuffer() {};
			virtual ~FrameBuffer() {};

			virtual DRReturn setup() = 0;
			virtual void release() = 0;
			virtual void bind() = 0;
			virtual bool isReady() = 0;
			__inline__ void setTexture(view::TexturePtr texture) { mTexture = texture; }
		protected:
			view::TexturePtr mTexture;

		};

	}
}



#endif //__UNIVERSUM_LIB_VIEW_GEOMETRIE_H