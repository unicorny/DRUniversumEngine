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

#ifndef __UNI_LIB_MODEL_MOVEABLE_NODE_H
#define __UNI_LIB_MODEL_MOVEABLE_NODE_H

#include "Node.h"
#include "Position.h"
#include "Rotation.h"

/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 15.11.2015
 *
 * \brief: base class for moveable scene nodes
 *
 */
namespace UniLib {
	namespace model {

		class UNIVERSUM_LIB_API MoveableNode : public Node
		{
		public:
			MoveableNode(Node* parent = NULL, const DRVector3& position = DRVector3(0.0f));
			virtual ~MoveableNode() {};

			__inline__ Position* getPosition() {return &mPosition;}
			__inline__ Rotation* getRotation() {return &mRotation;}

			virtual DRReturn move(float timeSinceLastFrame);

			void calculateMatrix();
			__inline__ const DRMatrix& getMatrix() const {return mMatrix;}
			
		protected:
			Position mPosition;
			Rotation mRotation;
			DRMatrix mMatrix;
		};

	}
}

#endif //__UNI_LIB_MODEL_MOVEABLE_NODE_H