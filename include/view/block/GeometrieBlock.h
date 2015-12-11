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


#ifndef __UNI_LIB_VIEW_BLOCK_GEOMETRIE_BLOCK_H
#define __UNI_LIB_VIEW_BLOCK_GEOMETRIE_BLOCK_H

#include "controller/GPUScheduler.h"
/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 11.12.2015
 *
 * \brief: view for geometrie blocks
 *  
 * used for instancing 
 * Observe Patterm Observe instances with same material and geometrie
 *
 */

namespace UniLib {
	namespace model {
		class UniformSet;
		class GeometrieBlock;
		typedef DRResourcePtr<GeometrieBlock> GeometrieBlockPtr;
	}
	namespace view {
		namespace geometrie {
			class BaseGeometrieContainer;
		}
		class Material;		
		namespace block {

			class UNIVERSUM_LIB_API GeometrieBlock : public controller::GPURenderCall
			{
			public:
				GeometrieBlock();
				virtual ~GeometrieBlock();

				virtual DRReturn render(float timeSinceLastFrame) = 0;
				// if render return not DR_OK, Call will be removed from List and kicked will be called
				virtual void kicked() = 0;  
				// will be called if render call need to much time
				// \param percent used up percent time of render main loop
				virtual void youNeedToLong(float percent) {};

				// remove all geometrie block ptr with ref = 1
				void clearGeometrieBlockList();
				__inline__ void addGeometrieBlock(model::GeometrieBlockPtr geometrieBlock) {mGeometrieBlockModels.push_back(geometrieBlock);}

				__inline__ size_t getGeometrieBlockCount() {return mGeometrieBlockModels.size();}
				
			protected:
				Material* mMaterial;
				geometrie::BaseGeometrieContainer* mGeometrie;
				model::UniformSet* mMatrixUniforms;
				std::list<model::GeometrieBlockPtr> mGeometrieBlockModels;
			};
		}
	}
}

#endif //__UNI_LIB_VIEW_BLOCK_GEOMETRIE_BLOCK_H