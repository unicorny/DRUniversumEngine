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
 * \author Dario Rekowski
 * 
 * \date 11.12.2015
 * 
 * \desc contain every active block type which can be rendered with instancing
 */

#ifndef _DR_UNIVERSUM_LIB_CONTROLLER_INSTANCING_BLOCK_MANAGER_H_
#define _DR_UNIVERSUM_LIB_CONTROLLER_INSTANCING_BLOCK_MANAGER_H_

#include "lib/Singleton.h"

namespace UniLib {
	namespace view {
		namespace block {
			class GeometrieBlock;
		}
	}
	namespace controller {
		class InstancingBlockManager : public lib::Singleton
		{
		public:
			// return the one and one instance (create if not exist)
			static InstancingBlockManager* getInstance();

			virtual view::block::GeometrieBlock* findGeometrieBlock(HASH id);
			void clearEmptyGeometrieBlocks();
		protected:
			InstancingBlockManager();
			virtual ~InstancingBlockManager();

			std::map<HASH, view::block::GeometrieBlock*> mGeometrieBlocks;
			typedef std::pair<HASH, view::block::GeometrieBlock*> GEOMETRIE_BLOCK_PAIR;

			static InstancingBlockManager* mpInstanz;
		};
	}
}

#endif //_DR_UNIVERSUM_LIB_CONTROLLER_INSTANCING_BLOCK_MANAGER_H_