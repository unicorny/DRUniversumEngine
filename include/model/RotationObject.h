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

#ifndef __UNI_LIB_MODEL_ROTATION_MODEL_OBJECT_H
#define __UNI_LIB_MODEL_ROTATION_MODEL_OBJECT_H

#include "Object.h"

namespace UniLib {
	namespace model {

		class UNIVERSUM_LIB_API RotationObject : public Object
		{
		public:
			RotationObject(Object* parent = NULL, DRVector3 pos = DRVector3(0.0f), DRVector3 scale = DRVector3(1.0f));
			virtual ~RotationObject();

			//! in Verh&auml;ltniss zum Objekteigenem Koordinatensystem
			//! \brief rotiert das DRObjekt relativ
			//! \param rotate die Rotation in Grad um die 3 Achsen
			void rotateRel(const DRVector3& rotate);
			//! in Verh&auml;ltniss zum Zentralem Koordinatensystem
			//! \brief rotiert das DRObjekt absolut
			//! \param rotate die Rotation in Grad um die 3 Achsen
			void rotateAbs(const DRVector3& rotate);

			void lookAt(DRVector3 targetPosition, DRVector3 upVector = DRVector3(0.0f, 1.0f, 0.0f));

			__inline__ DRVector3 getYAxis() {lock(); DRVector3 axis = mYAxis; unlock(); return axis;}
			__inline__ DRVector3 getXAxis() {lock(); DRVector3 axis = mXAxis; unlock(); return axis;}
			__inline__ DRVector3 getZAxis() {lock(); DRVector3 axis = mZAxis; unlock(); return axis;}

			void setAxis(DRVector3 XAxis, DRVector3 YAxis, DRVector3 ZAxis)
			{
				lock();
				mXAxis = XAxis;
				mYAxis = YAxis;
				mZAxis = ZAxis;
				unlock();
			}

		protected:
			DRVector3 mXAxis;
			DRVector3 mYAxis;
			DRVector3 mZAxis;
		};
		
	}
}



#endif //__UNI_LIB_MODEL_ROTATION_MODEL_OBJECT_H