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

			
			__inline__ block::BlockPtr operator[] (DRVector3i index) const
			{
				assert(index.x >= 0 && index.x < 9);
				assert(index.y >= 0 && index.y < 9);
				assert(index.z >= 0 && index.z < 9);
				return (*this)[DRMakeSmallVector3DHash(index)];
			}
			
			__inline__ bool isPlaceFree(DRVector3i index) const {return isPlaceFree(DRMakeSmallVector3DHash(index));}

			// return DR_ERROR if place is already occupied
			DRReturn addBlock(block::BlockPtr block, DRVector3i index);
			block::BlockPtr deleteBlock(DRVector3i index);
		protected:

			bool isPlaceFree(HASH h) const {return mBlocks.find(h) == mBlocks.end();}
			block::BlockPtr operator[] (HASH h) const;

			// hash calculated from 3D block index
			
			typedef std::map<HASH, block::BlockPtr> BlockMap;
			typedef std::pair<HASH, block::BlockPtr> BlockPair;
			typedef BlockMap::const_iterator BlockIterator;
			
			BlockMap mBlocks;
		};


	}
}

#endif //__UNI_LIB_MODEL_BLOCK_SEKTOR_H