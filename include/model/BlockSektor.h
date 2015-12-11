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


#ifndef __UNI_LIB_MODEL_BLOCK_SEKTOR_H
#define __UNI_LIB_MODEL_BLOCK_SEKTOR_H

#include "Sektor.h"
/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 11.12.2015
 *
 * \brief: class for sector containing blocks
 *
 */
namespace UniLib {
	namespace view {
		class BlockSektor;
	}
	namespace model {
		namespace block {
			class Block;
			typedef DRResourcePtr<Block> BlockPtr;
		}
		class UNIVERSUM_LIB_API BlockSektor : public Sektor 
		{
		public:
			BlockSektor(view::BlockSektor* viewSektor = NULL);
			virtual ~BlockSektor();

			
			__inline__ short operator[] (DRVector3i index) const {
				assert(index.x >= 0 && index.x < 9);
				assert(index.y >= 0 && index.y < 9);
				assert(index.z >= 0 && index.z < 9);
				return mBlockGrid[index.x][index.y][index.z];
			}
			__inline__ bool isPlaceFree(DRVector3i index) {return (*this)[index] == 0;}

			void addBlock(block::BlockPtr block, DRVector3i index);
		protected:
			//! \brief blocks
			std::map<short, block::BlockPtr> mBlocks;
			//! \brief position of blocks
			short mBlockGrid[9][9][9];
		};
	}
}

#endif //__UNI_LIB_MODEL_BLOCK_SEKTOR_H