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

#ifndef __UNI_LIB_VIEW_ROTATION_OBJECT_H
#define __UNI_LIB_VIEW_ROTATION_OBJECT_H

#include "model/Object.h"

namespace UniLib {
	
	namespace view {
		namespace geometrie {
			class BaseGeometrieContainer;
			typedef DRResourcePtr<BaseGeometrieContainer> BaseGeometrieContainerPtr;
		}
		class Material;
		typedef DRResourcePtr<Material> MaterialPtr;

		class UNIVERSUM_LIB_API Object: public model::Object
		{
		public:
			Object(model::Object* parent = NULL, DRVector3 pos = DRVector3(0.0f), DRVector3 scale = DRVector3(1.0f));
			virtual ~Object();

			__inline__ virtual bool isViewObject() {return true;}

			__inline__ void setGeometrie(geometrie::BaseGeometrieContainerPtr geo) {mGeometrie = geo;}
			__inline__ geometrie::BaseGeometrieContainerPtr getGeometrie() {return mGeometrie;}

			__inline__ void setMaterial(MaterialPtr material) {mMaterial = material;}
			__inline__ MaterialPtr getMaterial() {return mMaterial;}
			
		protected:
			geometrie::BaseGeometrieContainerPtr mGeometrie;				 
			MaterialPtr mMaterial;
		};
	}
}



#endif //__UNI_LIB_VIEW_ROTATION_OBJECT_H