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


#ifndef __UNI_LIB_MODEL_BLOCK_BLOCK_TYPE_H
#define __UNI_LIB_MODEL_BLOCK_BLOCK_TYPE_H

#include "UniversumLib.h"
/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 11.12.2015
 *
 * \brief: base class for block types, class for type
 *
 */

namespace UniLib {
	namespace model {
		namespace block {

			class UNIVERSUM_LIB_API BlockType : public DRIResource
			{
			public:
				BlockType(std::string name);
				virtual ~BlockType();

				virtual const char* getResourceType() const {return "BlockType";}
				virtual bool less_than(DRIResource& b) const {
					return mId <  dynamic_cast<BlockType&>(b).mId;
				}
			protected:
				int mId;
				std::string mName;
			};
		}
	}
}

#endif //__UNI_LIB_MODEL_BLOCK_BLOCK_TYPE_H