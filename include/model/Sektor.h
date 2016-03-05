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

#ifndef __DR_UNIVERSUM_LIB_MODEL_SEKTOR_H__ 
#define __DR_UNIVERSUM_LIB_MODEL_SEKTOR_H__

#include "Node.h"
#include "view/Sektor.h"


namespace UniLib {
	namespace view {
		class Camera;
	}
	namespace model {
		class SektorID;
		/*!
		 * Sektor model prototyp, contains active and inactive objects in sector,
		 * used for visible calculation
		 * management other classes which work with sektor (like view or generator)
		 */
		class UNIVERSUM_LIB_API Sektor : public Node
		{
		public:
			Sektor(Node* parent, SektorID* id, view::Sektor* sektorView = NULL);
			virtual ~Sektor();

			// prototypes
			// get name of sektor type
			virtual const char* getSektorType() = 0;
			// move/update objects in sektor
			virtual DRReturn move(float timeSinceLastFrame) = 0;
			// render sektor content on screen (using sektor view)
			// render sektor per camera (multiple calls pro frame possibility)
			__inline__ DRReturn render(view::Camera* camera, float timeSinceLastFrame) {return mSektorView->render(camera, timeSinceLastFrame);}


			__inline__ view::Sektor* getSektorView() {return mSektorView;}
			__inline__ SektorID* getSektorID() { return mID; }

		protected:
			view::Sektor* mSektorView;
			SektorID* mID;
			
		};
	};
};

#endif //__DR_UNIVERSUM_LIB_MODEL_SEKTOR_H__