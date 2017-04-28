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

#ifndef __UNIVERSUM_LIB_CONTROLLER_BIND_TO_RENDER_H
#define __UNIVERSUM_LIB_CONTROLLER_BIND_TO_RENDER_H
/*!
 * \author Dario Rekowski
 * 
 * \date 14.01.16
 * 
 * \desc Interface for render implemenattion callbacks
 * 
 */

#include "UniversumLib.h"


namespace UniLib {
	namespace view {
		class Material;
		class TextureMaterial;
		class MultiTextureMaterial;
		class BlockSektor;
		class Texture;
		typedef DRResourcePtr<Texture> TexturePtr;
		class Geometrie;
		class FrameBuffer;
	}
	namespace model {
		class Shader;
		class ShaderProgram;
		class UniformSet;
		namespace geometrie {
			class BaseGeometrie;
		}
	}

	namespace controller {
		class UNIVERSUM_LIB_API BindToRenderer
		{
		public: 
			virtual view::Material* newMaterial() = 0;
			virtual view::TextureMaterial* newTextureMaterial() = 0;
			virtual view::MultiTextureMaterial* newMultiTextureMaterial(size_t textureCount) = 0;
			virtual view::BlockSektor* newBlockSektor() = 0;
			virtual view::Texture* newTexture(DRVector2i size, GLuint format) = 0;
			virtual view::Texture* newTexture(DHASH id, const char* fileName) = 0;
			virtual view::Geometrie* newGeometrie(model::geometrie::BaseGeometrie* baseGeometrie) = 0;
			virtual view::FrameBuffer* newFrameBuffer(view::TexturePtr texture) = 0;
			virtual view::FrameBuffer* newFrameBuffer() = 0;
			//virtual model::geometrie::BaseGeometrieContainer* newGeometrieContainer() = 0;
			virtual model::Shader* newShader(HASH id) = 0;
			virtual model::ShaderProgram* newShaderProgram(const char* name, HASH id)  = 0;
			virtual model::UniformSet* newUniformSet() = 0;
			//virtual 

		};
	}
} 

#endif //__UNIVERSUM_LIB_CONTROLLER_BIND_TO_RENDER_H