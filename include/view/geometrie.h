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
 * \date: 23.01.2016
 *
 * \desc: Base interface for geometrie view, get geometrie datat to gpu and rendered
 */

#ifndef __UNIVERSUM_LIB_VIEW_GEOMETRIE_H
#define __UNIVERSUM_LIB_VIEW_GEOMETRIE_H

#include "UniversumLib.h"

namespace UniLib {
	namespace model {
		namespace geometrie {
			class BaseGeometrie;
		}
	}
	namespace view {

		class UNIVERSUM_LIB_API Geometrie : public DRIResource
		{
		public:
			Geometrie(model::geometrie::BaseGeometrie* baseGeometrie);
			virtual ~Geometrie();

			virtual DRReturn uploadToGPU() = 0;
			virtual DRReturn render() = 0;
			virtual bool isReady() = 0;
			__inline__ void setBaseGeometrie(model::geometrie::BaseGeometrie* baseGeometrie) { mGeometrieModel = baseGeometrie; }

			virtual const char* getResourceType() const {return "view::Geometrie";}
			// simple compare pointer adresses
			virtual bool less_than(DRIResource& b) const {return this < &b;}
		protected:
			model::geometrie::BaseGeometrie* mGeometrieModel;

		};

		typedef DRResourcePtr<Geometrie> GeometriePtr;
		
	}
}



#endif //__UNIVERSUM_LIB_VIEW_GEOMETRIE_H