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

#ifndef __UNI_LIB_MODEL_OBJECT_H
#define __UNI_LIB_MODEL_OBJECT_H

#include "lib/MultithreadContainer.h"

namespace UniLib {
	namespace model {

		class Object;
		typedef DRResourcePtr<Object> ObjectPtr; 

		class UNIVERSUM_LIB_API Object : public lib::MultithreadContainer
		{
		public:
			Object(Object* parent = NULL, DRVector3 pos = DRVector3(0.0f), DRVector3 scale = DRVector3(1.0f));
			virtual ~Object();

			__inline__ void setPosition(const DRVector3& pos) {lock(); mPosition = pos; unlock();}
			__inline__ DRVector3 getPosition() {lock(); DRVector3 pos = mPosition; unlock(); return pos;}

			__inline__ void setScale(const DRVector3& scale) {lock(); mScale = scale; unlock();}
			__inline__ DRVector3 getScale() {lock(); DRVector3 scale = mScale; unlock(); return scale;}

			__inline__ DRMatrix getMatrix() {mMatrixMutex.lock(); DRMatrix matrix = mMatrix; mMatrixMutex.unlock(); return matrix;}

			__inline__ Object* getParent() {return mParent;}
			__inline__ void addChild(Object* child) {mChilds.push_back(child);}
			//! \brief attention, not the fastest function
			__inline__ void removeChild(Object* child) {mChilds.remove(child);}

			__inline__ virtual bool isViewObject() {return false;}
			__inline__ virtual bool isControllerObject() {return false;}

		protected:
			//! parent object in scene graph
			Object* mParent;
			// position relative to parent
			DRVector3 mPosition;
			// scale relative to parent
			DRVector3 mScale;
			// matrix containing position and scale from this object and parent
			DRMatrix mMatrix;
			// child objects
			std::list<Object*> mChilds;

			lib::MultithreadContainer mMatrixMutex;
			
		};
		
	}
}



#endif //__UNI_LIB_MODEL_OBJECT_H