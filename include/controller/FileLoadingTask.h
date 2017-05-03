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
* \date: 29.04.17
*
* \desc: One generic Task for loading one or more files from harddisk, using the harddisk scheduler
*/

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_FILE_LOADING_TASK_H__
#define __DR_UNIVERSUM_LIB_CONTROLLER_FILE_LOADING_TASK_H__

#include "CPUTask.h"

namespace UniLib {
	namespace controller {

		class UNIVERSUM_LIB_API FileLoadingReciver
		{
		public:
			// if returns true, FileLoadingTask delete data buffer
			virtual bool getFileFromMemory(DRVirtualFile* fileInMemory) { return true; }
			virtual bool getFileFromMemory(DRVirtualFile** filesInMemory, size_t fileCount) { return true; }
			// called after task was finished, to make it possible to update CPU scheduler
			virtual void finishFileLoadingTask() = 0;
		};

		class CPUSheduler;
		class UNIVERSUM_LIB_API FileLoadingTask : public CPUTask
		{
		public:
			FileLoadingTask(FileLoadingReciver* reciver, const char* fileName);
			FileLoadingTask(FileLoadingReciver* reciver, std::vector<std::string> fileNames);
			virtual ~FileLoadingTask();

			virtual const char* getResourceType() const { return "FileLoadingTask"; };
			//! \brief return true if task has finished, else false
			//! automatic scheduling of task if he isn't finished and sheduled yet
			virtual bool isTaskFinished();

			virtual DRReturn run();
		protected:
			// input data
			FileLoadingReciver* mFinishReciver;
			std::vector<std::string> mFileNames;

			// output data
			DRVirtualFile** mFilesInMemory;
			bool		mFinished;
		};
	}
}

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_FILE_LOADING_TASK_H__