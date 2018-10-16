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

\brief Container class for mutex protected map

\author Dario Rekowski

\date 27.05.2016
*/

#ifndef __DR_UNIVERSUM_LIB_LIB_MULTITHREAD_MAP_H
#define __DR_UNIVERSUM_LIB_LIB_MULTITHREAD_MAP_H

#include "UniversumLib.h"

namespace UniLib {
	namespace lib {

		template <class KeyType, class ResourceType>
		class MultithreadMap : public std::map<KeyType, ResourceType>, public MultithreadContainer
		{
		public: 
			virtual ~MultithreadMap() {
				s_clear();
			}
			void s_clear()
			{
				lock();
				for (iterator it = begin(); it != end(); it++) {
					DR_SAVE_DELETE(it->second);
				}
				clear();
				unlock();
			}
			DRReturn s_add(KeyType key, ResourceType resource, ResourceType* dublette) {
				lock();
				iterator it = find(key);
				if (it != end()) {
					// maybe a Hash collision?
					if (!(it->second == resource)) {
						if (dublette) dublette = &it->second;
						unlock();
						LOG_ERROR("hash collision!", DR_ERROR);
					}
					unlock();
					// element already in map
					return DR_OK;
				}
				else {
					insert(std::pair<KeyType, ResourceType>(key, resource));
					unlock();
					return DR_OK;
				}
				unlock();
				return DR_ERROR;
			}
			ResourceType s_find(KeyType key) {
				lock();
				iterator it = find(key);
				if (it != end()) {
					unlock();
					return it->second;
				}
				unlock();
				return NULL;
			}
			ResourceType s_remove(KeyType key) {
				lock();
				iterator it = find(key);
				if (it != end()) {
					ResourceType result = it->second;
				    erase(it);
					unlock();
					return result;
				}
				unlock();
				return NULL;
			}

		};
	}
}

#endif __DR_UNIVERSUM_LIB_LIB_MULTITHREAD_MAP_H