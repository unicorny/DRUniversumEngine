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

#ifndef __UNIVERSUM_LIB_CONTROLLER_BLOCK_MATERIAL_MANAGER_H
#define __UNIVERSUM_LIB_CONTROLLER_BLOCK_MATERIAL_MANAGER_H
/*!
 * \author Dario Rekowski
 * 
 * \date 07.01.16
 * 
 * \desc Manager for all Material Types in Game, import material template from json
 * 
 */

#include "lib/Singleton.h"
#include "controller/CPUTask.h"

namespace UniLib {
	namespace model {
		namespace block {
			class BlockType;	
		}
	}
	namespace controller {
		class CPUSheduler;

		// task for parsing block type data
		class BlockTypeLoadingTask: public CPUTask
		{
		public:
			BlockTypeLoadingTask(CPUSheduler* scheduler, std::list<std::string>* fileContents)
				: CPUTask(scheduler), mFileContents(fileContents) {}

			virtual DRReturn run();
			virtual bool isTaskFinished() {return false;};
			virtual const char* getResourceType() const { return "BlockTypeLoadingTask"; };
		protected:
			std::list<std::string>* mFileContents;


		};
		// task for loading files from harddisk
		class LoadingJsonFilesIntoMemoryTask : public CPUTask
		{
		public:
			LoadingJsonFilesIntoMemoryTask(const std::list<std::string>* filenames, CPUSheduler* schedulerForParser)
				: CPUTask(g_HarddiskScheduler), mFileNames(*filenames), mSchedulerForParser(schedulerForParser) {}

			virtual DRReturn run();
			virtual bool isTaskFinished() { return false; };
			virtual const char* getResourceType() const { return "LoadingJsonFilesIntoMemoryTask"; };
		protected:
			std::list<std::string> mFileNames;
			CPUSheduler* mSchedulerForParser;
		};

		class UNIVERSUM_LIB_API BlockTypeManager : public lib::Singleton
		{
			friend BlockTypeLoadingTask;
			friend LoadingJsonFilesIntoMemoryTask;
		public:
			static BlockTypeManager* getInstance();
			DRReturn init(const std::list<std::string>* filenames);
			DRReturn initAsyn(const std::list<std::string>* filenames, CPUSheduler* scheduler);
			// calling after every object using MaterialBlocks was cleaned up
			void exit();

			__inline__ LoadingState checkLoadingState() { mWorkMutex.lock(); LoadingState val = mLoadingState; mWorkMutex.unlock(); return val; }
			__inline__ model::block::BlockType* getBlockType(HASH id);
			__inline__ model::block::BlockType* getBlockType(const char* name) { return getBlockType(DRMakeStringHash(name));}


		protected:
			BlockTypeManager();
			virtual ~BlockTypeManager();

			DRReturn _loadingFilesIntoMemory(const std::list<std::string>* fileNames, std::list<std::string>* fileContentsReturn);
			DRReturn _parsingJsonToBlockTypes(const std::list<std::string>* mFilesContent);

			// member variables
			lib::MultithreadContainer mWorkMutex;
			typedef std::map<HASH, model::block::BlockType*> BlockTypeMap;
			typedef std::pair<HASH, model::block::BlockType*> BlockTypePair;
			typedef BlockTypeMap::iterator BlockTypelIter;
			BlockTypeMap mBlockTypes;

		private:
			LoadingState mLoadingState;
			

		};
	}
}

#endif //__UNIVERSUM_LIB_CONTROLLER_BLOCK_MATERIAL_MANAGER_H