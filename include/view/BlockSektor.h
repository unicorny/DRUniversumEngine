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


#ifndef __UNI_LIB_VIEW_BLOCK_SEKTOR_H
#define __UNI_LIB_VIEW_BLOCK_SEKTOR_H

#include "Sektor.h"
/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 11.12.2015
 *
 * \brief: view class for sector containing blocks
 *
 */
namespace UniLib {
	namespace model {
		class BlockSektor;
	}
	namespace view {

		class UNIVERSUM_LIB_API BlockSektor : public Sektor 
		{
		public:
			BlockSektor(model::BlockSektor* sektorModel = NULL);
			virtual ~BlockSektor();

			// calculate current visibility mode for given camera, multiple calls per frame possible
			virtual DRReturn updateVisibility(view::Camera* camera);
			// render it, maybe
			virtual DRReturn render(view::Camera* camera, float timeSinceLastFrame);
		protected:

		};
	}
}

#endif //__UNI_LIB_MODEL_BLOCK_SEKTOR_H