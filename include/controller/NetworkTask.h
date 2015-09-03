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
 *
 * \author: Dario Rekowski
 * 
 * \date: 31.08.15
 *
 * \desc: One Task inside of the network, waiting to be processed (mainly a request)
 */

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_NETWORK_TASK_H__
#define __DR_UNIVERSUM_LIB_CONTROLLER_NETWORK_TASK_H__

#include "Task.h"


namespace UniLib {
    namespace controller {

        class UNIVERSUM_LIB_API NetworkTask;
        typedef DRResourcePtr<NetworkTask> NetworkTaskPtr;

        class UNIVERSUM_LIB_API NetworkTask : public Task
        {
        public: 
            NetworkTask();
            ~NetworkTask();
        protected:

        };
    }
}

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_NETWORK_TASK_H__
        