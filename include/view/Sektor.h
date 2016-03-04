/*/*************************************************************************
 *                                                                         *
 * UniversumLib, collection of classes for generating and go through a     *
 * whole universe. It is for my Gameproject Spacecraft					   * 
 * Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
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

#ifndef __DR_UNIVERSUM_LIB_VIEW_SEKTOR_GENERATOR_H__ 
#define __DR_UNIVERSUM_LIB_VIEW_SEKTOR_GENERATOR_H__

#include "UniversumLib.h"

// Prototyp class for view classes,
// renderSektor in old implemantation

namespace UniLib {
	namespace model {
		class Sektor;
	}
	namespace view {

		class Camera;

		class UNIVERSUM_LIB_API Sektor
		{
		public:
			Sektor(model::Sektor* sektorModel = NULL);
			~Sektor();

			__inline void setSektorModel(model::Sektor* sektorModel) {mSektorModel = mSektorModel;}

			// prototypes
			// calculate current visibility mode for given camera, multiple calls per frame possible
			virtual DRReturn updateVisibility(view::Camera* camera) = 0;
			// render sektor per camera (multiple calls pro frame possible)
			virtual DRReturn render(view::Camera* camera, float timeSinceLastFrame) = 0;

			
		protected:
			model::Sektor* mSektorModel;
		private:
		};
	};
};

#endif //__DR_UNIVERSUM_LIB_VIEW_SEKTOR_GENERATOR_H__