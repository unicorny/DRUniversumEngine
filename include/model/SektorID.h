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

/**
 * \author Dario Rekowski
 * 
 * \date 19.03.15
 * 
 * \desc SektorID, a unique ID for every sektor, contains complete sektor path
 */

#ifndef __DR_UNIVERSUM_LIB_MODEL_SEKTOR_ID_H__ 
#define __DR_UNIVERSUM_LIB_MODEL_SEKTOR_ID_H__

#include "UniversumLib.h"

namespace UniLib {
    namespace model {
		enum UNIVERSUM_LIB_API SektorIdType {
			SEKTOR_ID_DEFAULT = 0,
			SEKTOR_ID_BLOCK_SEKTOR = 1,
			SEKTOR_ID_BLOCK_SEKTOR_TREE = 2
		};

        class UNIVERSUM_LIB_API SektorID
        {
        public:
			SektorID(u8 id, SektorIdType type, SektorID* parent = NULL);
			SektorID(u16 id, SektorIdType type, SektorID* parent = NULL);
			SektorID(u32 id, SektorIdType type, SektorID* parent = NULL);
			SektorID(u64 id, SektorIdType type, SektorID* parent = NULL);
			SektorID(u8* id, u8 arraySize, SektorIdType type, SektorID* parent = NULL);
			
			virtual ~SektorID();

			// fill buffer with serialized id
			DRReturn serialize(u8* buffer);
			int getSeralizedSize();

			// create Sektor ID Object tree from id
			static SektorID* createSektorID(u8* id, u8 arraySize, SektorID* parent = NULL);

			__inline__ void setParent(SektorID* parent) { mParent = parent; }

        protected:
			u8* mID;
			u8 mArraySize;
			u8 mType;
			SektorID* mParent;
        };
    }
}

#endif //__DR_UNIVERSUM_LIB_MODEL_SEKTOR_ID_H__