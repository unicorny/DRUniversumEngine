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


#ifndef __UNI_LIB_DEBUG_CPU_SCHEDULER_TASKS_LOG_H
#define __UNI_LIB_DEBUG_CPU_SCHEDULER_TASKS_LOG_H
#include "lib/Singleton.h"
#include "lib/MultithreadContainer.h"
#ifdef _UNI_LIB_DEBUG
namespace UniLib {
	namespace debug {
		class UNIVERSUM_LIB_API CPUShedulerTasksLog : public lib::Singleton, public lib::MultithreadContainer
		{
		public:
			static CPUShedulerTasksLog* const getInstance();
			__inline__ static bool	isInitialized() { return getInstance()->mInitialized; };

			//! \brief init 
			//! \param defaultCPUSheduler CPUSheduler on which texture loading and saving take place
			//! \param updateTimer timer on which to attach for updating texture storage
			//! \param rerunDelay how much ms passes between update calls
			DRReturn init();
			void exit();

			void addTaskLogEntry(HASH id, const char* resourcesTypeName, const char* threadName, const char* name = NULL);
			void removeTaskLogEntry(HASH id);

			void printCurrentlyRunningTasks();
			std::string getCurrentlRunningTasksTableString();

		protected:
			CPUShedulerTasksLog();
			virtual ~CPUShedulerTasksLog();

			bool mInitialized;

			struct TaskLogEntry {
				TaskLogEntry(HASH id, const char* typeName, const char* threadName, const char* name)
					: resourceTypeName(typeName), name(name), threadName(threadName) {}
				std::string resourceTypeName;
				std::string name;
				std::string threadName;
				HASH id;
			};
			typedef std::map<HASH, TaskLogEntry*> TaskLogEntryMap;
			typedef std::pair<HASH, TaskLogEntry*> TaskLogEntryPair;
			TaskLogEntryMap mTaskLogEntrys;

		};
	}
}
#endif // _UNI_LIB_DEBUG

#endif //__UNI_LIB_DEBUG_CPU_SCHEDULER_TASKS_LOG_H