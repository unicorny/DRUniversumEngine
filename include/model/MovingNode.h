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

#ifndef __UNI_LIB_MODEL_MOVING_NODE_H
#define __UNI_LIB_MODEL_MOVING_NODE_H

#include "Position.h"
#include "Rotation.h"
#include "MoveableNode.h"

/*!
*
* \author: Dario Rekowski
*
* \date: 28.04.2017
*
* \brief: base class for moving node, update every frame, with velocity
*
*/
namespace UniLib {
	namespace model {

		class UNIVERSUM_LIB_API MovingNode: public MoveableNode
		{
		public:
			MovingNode(MovingNode* parent = NULL, const DRVector3& position = DRVector3(0.0f));
			virtual ~MovingNode() {};

			__inline__ void setVelocity(const DRVector3& vel) { mVelocity = vel; }
			__inline__ void changeVelocity(const DRVector3& diff) { mVelocity += diff; }
			__inline__ DRVector3 getVelocity() { return mVelocity; }
			__inline__ const DRVector3& getVelocity() const { return mVelocity; }

			__inline__ void setRotationVelocity(const DRVector3& vel) { mRotationVelocity = vel; }
			__inline__ void changeRotationVelocity(const DRVector3& diff) { mRotationVelocity += diff; }
			__inline__ DRVector3 getRotationVelocity() { return mRotationVelocity; }
			__inline__ const DRVector3& getRotationVelocity() const { return mRotationVelocity; }

			virtual DRReturn move(float timeSinceLastFrame);

		protected:
			DRVector3 mVelocity;
			DRVector3 mRotationVelocity;
		};

	}
}

#endif //__UNI_LIB_MODEL_MOVEABLE_NODE_H