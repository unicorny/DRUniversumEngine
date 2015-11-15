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

#ifndef __UNI_LIB_VIEW_VISIBLE_NODE_H
#define __UNI_LIB_VIEW_VISIBLE_NODE_H

#include "model/MoveableNode.h"

/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 15.11.2015
 *
 * \brief: base class for visible scene nodes
 *
 */
namespace UniLib {
	namespace view {

		namespace geometrie {
			class BaseGeometrieContainer;
			typedef DRResourcePtr<BaseGeometrieContainer> BaseGeometrieContainerPtr;
		}
		class Material;
		typedef DRResourcePtr<Material> MaterialPtr;

		class UNIVERSUM_LIB_API VisibleNode : public model::MoveableNode
		{
		public:
			VisibleNode(Node* parent = NULL, const DRVector3& position = DRVector3(0.0f));
			virtual ~VisibleNode() {};

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

#endif //__UNI_LIB_VIEW_VISIBLE_NODE_H