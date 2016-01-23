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

//#include "UniversumLib.h"
#include "lib/MultithreadContainer.h"
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

			enum BlockTypeLoadingState {
				// empty structure
				BLOCK_TYPE_EMPTY = 0,
				// has every information needed to load
				BLOCK_TYPE_HAS_INFORMATIONS = 1,
				// work on loading resources
				BLOCK_TYPE_PARTLY_LOADED = 2,
				// fully loaded and ready for using
				BLOCK_TYPE_FULLY_LOADED = 4
			};

			enum BlockBaseType {
				BLOCK_BASE_TYPE_SOLID = 0,
				BLOCK_BASE_TYPE_FLUENT = 1,
				BLOCK_BASE_TYPE_GAS = 2
			};

			

			class UNIVERSUM_LIB_API BlockType : public DRIResource, protected lib::MultithreadContainer
			{
			public:
				BlockType(std::string name);
				virtual ~BlockType();

				virtual const char* getResourceType() const {return "BlockType";}
				virtual bool less_than(DRIResource& b) const {
					return mId <  dynamic_cast<BlockType&>(b).mId;
				}

				// getter
				__inline__ bool isTransparency() {return mTransparency;}
				__inline__ const char* getName() {return mName.data();}
				__inline__ float getDensity() {return mDensity;}
				__inline__ float getMeltingPoint() {return mMeltingPoint;}
				__inline__ int getHitPoints() {return mHitpoints;}

				// setter 
				__inline__ void setId(HASH id) {mId = id;}
			protected:
				// control
				int mId;
				BlockTypeLoadingState mLoadingState;

				// block attributes
				std::string mName;
				BlockBaseType mBaseType;
				bool mTransparency;
				float mDensity;
				float mMeltingPoint;
				int mHitpoints;

				// string to enum, enum to string functions
				static BlockBaseType getBlockBaseTypeEnum(std::string blockBaseTypeString);

			};
		}
	}
}

#endif //__UNI_LIB_MODEL_BLOCK_BLOCK_TYPE_H