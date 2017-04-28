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



#ifndef __UNI_LIB_MODEL_ROTATION_H
#define __UNI_LIB_MODEL_ROTATION_H

#include "UniversumLib.h"


/*!
 *
 * \author: Dario Rekowski
 *
 * \date 15.11.2015
 *
 * \brief simply represent a rotation
 *
 *
 */

namespace UniLib {
	namespace model {

		class UNIVERSUM_LIB_API Rotation 
		{
		public:
			Rotation(DRVector3 xAxis = DRVector3(1.0f, 0.0, 0.0f),
				     DRVector3 yAxis = DRVector3(0.0f, 1.0f, 0.0f),
					 DRVector3 zAxis = DRVector3(0.0, 0.0f, 1.0f));
			~Rotation();

			//! in Verh&auml;ltniss zum Objekteigenem Koordinatensystem
			//! \brief rotiert das DRObjekt relativ
			//! \param rotate die Rotation in Grad um die 3 Achsen
			// using no reference because of caching
			void rotateRel(DRVector3 rotate);
			//! in Verh&auml;ltniss zum Zentralem Koordinatensystem
			//! \brief rotiert das DRObjekt absolut
			//! \param rotate die Rotation in Grad um die 3 Achsen
			// using no reference because of caching
			void rotateAbs(DRVector3 rotate);

			// using no reference because of caching
			void lookAt(DRVector3 currentPos, DRVector3 targetPosition, DRVector3 upVector = DRVector3(0.0f, 1.0f, 0.0f));

			__inline__ const DRVector3& getYAxis() const {return mYAxis;}
			__inline__ DRVector3 getYAxis() {return mYAxis;}
			__inline__ const DRVector3& getXAxis() const {return mXAxis;}
			__inline__ DRVector3 getXAxis() {return mXAxis;}
			__inline__ const DRVector3& getZAxis() const {return mZAxis;}
			__inline__ DRVector3 getZAxis() {return mZAxis;}

			void setAxis(const DRVector3& XAxis, const DRVector3& YAxis, const DRVector3& ZAxis)
			{
				mXAxis = XAxis;
				mYAxis = YAxis;
				mZAxis = ZAxis;
			}

			DRMatrix calculateMatrix();

		protected:
			DRVector3 mXAxis;
			DRVector3 mYAxis;
			DRVector3 mZAxis;
		};
	}

}

#endif //__UNI_LIB_MODEL_ROTATION_H