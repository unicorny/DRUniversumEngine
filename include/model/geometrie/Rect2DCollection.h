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
* \date: 26.07.2019
*
* \desc Base class for geometrie models
*
*/

#ifndef __UNIVERSUM_LIB_MODEL_GEOMETRIE_RECT2D_COLLECTION__H
#define __UNIVERSUM_LIB_MODEL_GEOMETRIE_RECT2D_COLLECTION__H

#include "BaseGeometrie.h" 
#include "Core2/DRPack2D.h"

namespace UniLib {

	namespace model {
		namespace geometrie {

			class UNIVERSUM_LIB_API Rect2DCollection : public BaseGeometrie
			{
			public:
				Rect2DCollection();
				virtual ~Rect2DCollection();

				DRReturn Rect2DCollection::generateVertices(std::vector<DRPack2D_Size_WithKey>* inputRects);
				__inline__ DRVector2i getCollectionDimension() { return mCollectionDimension; }

				virtual const char* getResourceType() const { return "model::geometrie::Rect2DCollection"; }

			protected:
				DRVector2i mCollectionDimension;

			};
		}
	}
}


#endif //__UNIVERSUM_LIB_MODEL_GEOMETRIE_RECT2D_COLLECTION__H