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
* \date: 20.03.2016
*
* \desc: Material with multiple textures
*/

#ifndef __UNIVERSUM_LIB_VIEW_MULTI_TEXTURE_MATERIAL_H
#define __UNIVERSUM_LIB_VIEW_MULTI_TEXTURE_MATERIAL_H

#include "Material.h"

namespace UniLib {

	namespace view {

		class Texture;
		typedef DRResourcePtr<Texture> TexturePtr;

		class UNIVERSUM_LIB_API MultiTextureMaterial : public Material
		{
		public:
			MultiTextureMaterial(size_t textureCount);
			virtual ~MultiTextureMaterial();

			virtual const char* getResourceType() const { return "TextureMaterial"; }
			virtual bool hasMultiTexture() { return true; }

			__inline__ void setTexture(TexturePtr texture, size_t index) { assert(index < mTextureCount); mTextures[index] = texture; }
			void usingTexture(const char* filename, size_t index);
			__inline__ TexturePtr getTexture(size_t index) { assert(index < mTextureCount); return mTextures[index]; }
		protected:
			TexturePtr* mTextures;
			size_t		mTextureCount;
		};
	}
}

#endif //__UNIVERSUM_LIB_VIEW_MULTI_TEXTURE_MATERIAL_H