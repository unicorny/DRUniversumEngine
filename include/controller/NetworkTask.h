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
#include "lib/Structures.h"
#include "server/RequestCommand.h"

namespace UniLib {
    namespace controller {

        class UNIVERSUM_LIB_API NetworkTask;
        typedef DRResourcePtr<NetworkTask> NetworkTaskPtr;

        class UNIVERSUM_LIB_API NetworkTask : public Task, public server::RequestCommand
        {
        public: 
            NetworkTask(DRNetRequest& request, u16 connectionNumber);
			NetworkTask(size_t taskDependenceCount, u16 connectionNumber);
            virtual ~NetworkTask();

			//! \brief return true if task has finished, else false
			//! automatic scheduling of task if he isn't finished and sheduled yet
			virtual bool isTaskFinished();
			//! \brief called from task scheduler, maybe from another thread
			virtual DRReturn run();
			//! \brief called when net request receive data
			virtual void execute(DRNet_Status status, std::string& data);

			virtual const char* getResourceType() const {return "NetworkTask";};
			virtual bool const isNetworkTask() const {return true;}

			__inline__ std::string getResult() {return mResult;}

			virtual void scheduleTask(TaskPtr own) { run(); taskScheduled(); };
        protected:
			__inline__ bool isRequestSend() {return mRequestSent;}
			
			// result string
			std::string mResult;
			// request
			DRNetRequest mRequest;
			u16			mConnectionNumber;
		private: 
			bool mRequestSent;
        };
    }
}

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_NETWORK_TASK_H__
        