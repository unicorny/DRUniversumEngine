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
#ifndef __UNIVERSUM_LIB_LIB_COMMAND_EVENT_MANAGER_H__
#define __UNIVERSUM_LIB_LIB_COMMAND_EVENT_MANAGER_H__

//#include "UniversumLib.h"
#include "controller/Command.h"

namespace UniLib {
	namespace controller {
		class Command;
	}
	namespace lib {

		

		class UNIVERSUM_LIB_API CommandEventManager
		{
		public:
			CommandEventManager();
			~CommandEventManager();

			DRReturn init();
			void exit();

			DRReturn addCommandForEvent(const char* eventName, controller::Command* command);
			DRReturn addCommandForEvent(HASH eventHash, controller::Command* command);
			void removeCommandForEvent(const char* eventName, controller::Command* command);
			void removeCommandForEvent(HASH eventHash, controller::Command* command);

			DRReturn triggerEvent(const char* name, controller::Task* task);
			DRReturn triggerEvent(HASH eventHash, controller::Task* task);

		protected:
			typedef std::list<controller::Command*> CommandList;
			std::map<HASH, CommandList> mCommandsMap;
			typedef std::pair<HASH, CommandList> CommandMapPair;
			typedef std::map<HASH, CommandList>::iterator CommandMapIterator;
#ifdef _DEBUG
			// dubletten check
			typedef std::map<HASH, std::string> HashNameMap;
			typedef HashNameMap::iterator HashNameMapIterator;
			typedef std::pair<HASH, std::string> HashNameMapPair;
			HashNameMap mHashNameMap;
			
#endif
			// mutex
			SDL_mutex* mWorkingMutex;
		};

		class UNIVERSUM_LIB_API CallEventManagerCommand : public controller::Command
		{
		public:
			CallEventManagerCommand(const char* eventName, controller::Task* task, CommandEventManager* eventManager)
				: mEventName(eventName), mFinishedTask(task), mEventManagerToCall(eventManager) {assert(eventManager != NULL);}
			virtual DRReturn taskFinished(controller::Task* task) {return mEventManagerToCall->triggerEvent(mEventName.data(), mFinishedTask);}
		protected:
			std::string mEventName;
			controller::Task* mFinishedTask;
			CommandEventManager* mEventManagerToCall;
		};
	}
}


#endif