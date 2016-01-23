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

/*!
 * \author: Dario Rekowski
 *
 * \date: 31.10.2015
 * 
 * \desc: class for representing a plane geometrie
 *
 */

#ifndef __UNIVERSUM_LIB_MODEL_GEOMETRIE_PLANE_H
#define __UNIVERSUM_LIB_MODEL_GEOMETRIE_PLANE_H

#include "BaseGeometrie.h"

namespace UniLib {
	namespace model {
		namespace geometrie {

			class UNIVERSUM_LIB_API Plane : public BaseGeometrie
			{
			public:
				Plane(GeometrieDataType type);
				virtual ~Plane();

				virtual void generateVertices(GeometrieDataType type);
			protected:

			};
		}
	}
}
#endif //__UNIVERSUM_LIB_MODEL_GEOMETRIE_PLANE_H
