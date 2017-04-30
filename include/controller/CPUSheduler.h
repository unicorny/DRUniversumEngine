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
 * \date: 27.09.15
 *
 * \desc: Scheduler for CPU Tasks, multiple threads
 */

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_CPU_SHEDULER_H__
#define __DR_UNIVERSUM_LIB_CONTROLLER_CPU_SHEDULER_H__

#include "UniversumLib.h"

namespace UniLib {
    namespace controller {

		class UNIVERSUM_LIB_API Task;
		typedef DRResourcePtr<Task> TaskPtr;

		class CPUShedulerThread;

        class UNIVERSUM_LIB_API CPUSheduler 
        {
        public: 
			// \param threadCount how many threads should be used
			// \param name name for threads (only first 7 chars are used for thread name)
            CPUSheduler(u8 threadCount, const char* name);			
            virtual ~CPUSheduler();
			
			DRReturn sheduleTask(TaskPtr task); 
			void checkPendingTasks();
#ifdef _UNI_LIB_DEBUG
			CPUShedulerThread** getThreads(u8& count) {count = mThreadCount; return mThreads;};
#endif
			// called from scheduler thread if he wants a new task to do
			// return null if no task pending, putting thread in wait queue,
			// to inform him if a new task is ready for him
			TaskPtr getNextUndoneTask(CPUShedulerThread* Me);

			__inline__ u8 getThreadCount() { return mThreadCount; }
        protected:
			
			
		private: 
			// worker threads
			CPUShedulerThread** mThreads;
			u8			 mThreadCount;
			std::string	 mName;
			// free worker
			lib::MultithreadQueue<CPUShedulerThread*> mFreeWorkerThreads;
			// work to do
			//lib::MultithreadQueue<TaskPtr> mPendingTasks;
			std::list<TaskPtr> mPendingTasks;
			lib::MultithreadContainer mPendingTasksMutex;

        };
    }
}

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_CPU_SHEDULER_H__
        