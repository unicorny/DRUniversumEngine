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
 * \desc: Texture Model, contains texture pixel data
 *
 */

#ifndef __DR_UNIVERSUM_LIB_MODEL_TEXTURE_H
#define __DR_UNIVERSUM_LIB_MODEL_TEXTURE_H

#include "UniversumLib.h"
#include "lib/DRIImage.h"
#include "lib/MultithreadContainer.h"

class DRIImage;

namespace UniLib {
	namespace model {
		class UNIVERSUM_LIB_API Texture : public lib::MultithreadContainer
		{
		public:
			Texture();
			Texture(DRVector2i size, GLenum format);
			virtual ~Texture();

			// tasks
			DRReturn loadFromFile(const char* filename);
			//! \brief load image from memory
			DRReturn loadFromMemory(u8* data);
			DRReturn saveIntoFile(const char* filename, u8* data);
			DRReturn saveIntoFile(const char* filename, DRColor* color);
			DRReturn saveIntoFile(const char* filename);

			void clearMemory();

			// access data
			__inline__ DRVector2i getSize() { return mSize; }
			__inline__ void setSize(DRVector2i size) { mSize = size; }
			__inline__ GLenum getFormat() { return mFormat; }
			__inline__ DHASH getHash() { return calculateHash(mSize, mFormat); }
			__inline__ bool hasImageData() { return mImage != NULL; }
			__inline__ u8* getPixels() { assert(mImage != NULL); return mImage->getPixel(); }

			static DHASH calculateHash(DRVector2i size, GLenum format);

		protected:
			DRIImage* mImage;
			DRVector2i mSize;
			GLenum mFormat;
		};
	}
}

#endif //__DR_UNIVERSUM_LIB_MODEL_TEXTURE_H