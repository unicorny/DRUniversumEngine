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
 * \date: 17.10.2015
 * 
 * \desc: GPU Scheduler control all gpu processing, inclusive the main game loop (GPU part)
 *        Maybe whole OSG code must run in GPU Thread
 *
 */

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_GPU_SCHEDULER_H
#define __DR_UNIVERSUM_LIB_CONTROLLER_GPU_SCHEDULER_H

#include "lib/Singleton.h"
#include "lib/Thread.h"

namespace UniLib {
	
	namespace controller {

	class Task;
	typedef DRResourcePtr<Task> TaskPtr;
#define GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT 20
		
		class UNIVERSUM_LIB_API GPURenderCall 
		{
		public: 
			virtual DRReturn render(float timeSinceLastFrame) = 0;
			// if render return not DR_OK, Call will be removed from List and kicked will be called
			virtual void kicked() = 0;  
			// will be called if render call need to much time
			// \param percent used up percent time of render main loop
			virtual void youNeedToLong(float percent) = 0;

			virtual const char* getName() const { return ""; }
		};

		enum GPUSchedulerCommandType {
			GPU_SCHEDULER_COMMAND_BEFORE_PREPARE_RENDERING = 0,
			GPU_SCHEDULER_COMMAND_PREPARE_RENDERING = 1,
			GPU_SCHEDULER_COMMAND_RENDERING = 2,
			GPU_SCHEDULER_COMMAND_AFTER_RENDERING = 3,
			GPU_SCHEDULER_COMMAND_AFTER_AFTER_RENDERING = 4,
			GPU_SCHEDULER_COMMAND_MAX = 5
		};

		class UNIVERSUM_LIB_API GPUScheduler: public lib::Singleton {
		public:
			static GPUScheduler* const getInstance();
			static const int GPUTaskTargetTimes[];

			void registerGPURenderCommand(GPURenderCall* renderCall, GPUSchedulerCommandType type);
			void unregisterGPURenderCommand(GPURenderCall* renderCall, GPUSchedulerCommandType type);
			
			void addGPUTask(TaskPtr task, GPUTaskSpeed speed = GPU_TASK_SLOW);

			void startThread(const char* name = "UniGPUSch");
			void stopThread();

			__inline__ void lock() {SDL_LockMutex(mMutex);}
			__inline__ void unlock() {SDL_UnlockMutex(mMutex);}

			// call frame buffer activities, call main gpu game loop, variable frame rate
			static int run(void* data);
			// main render function, called from thread or from game
			DRReturn updateEveryRendering();

			// update GPU task from one task Speed Type
			void runGPUTasks(GPUTaskSpeed taskSpeed);

			__inline__ float getSecondsSinceLastFrame() {float seconds = 0.0f; SDL_LockMutex(mFrameTimeMutex); seconds = mSecondsSinceLastFrame; SDL_UnlockMutex(mFrameTimeMutex); return seconds;}

			// debugging/profiling
#ifdef _UNI_LIB_DEBUG
			__inline__ size_t getGPUTaskCount(GPUTaskSpeed taskSpeed) { return mGPUTasks[taskSpeed].size(); }
			__inline__ size_t getGPURenderCommandCount(GPUSchedulerCommandType type) {return mGPURenderCommands[(int)type].size();}
#endif 
		protected:

			GPUScheduler();
			virtual ~GPUScheduler();

			std::list<GPURenderCall*> mGPURenderCommands[GPU_SCHEDULER_COMMAND_MAX];
			std::queue<TaskPtr>	    mGPUTasks[GPU_TASK_ENTRY_COUNT];
			//std::queue<>
			bool	mThreadRunning;
			SDL_mutex* mMutex;
			SDL_mutex* mFrameTimeMutex;
			SDL_Thread* mThread;
			Uint32    mLastUpdateTicks;
			Uint32    mLastFrameDurations[GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT];
			Uint32    mLastFrameDurationCursor;

			float mSecondsSinceLastFrame;

			static const char* getGPUCommandTypeString(GPUSchedulerCommandType type);
			

		private:
			static GPUScheduler* mpInstanz;
		};
	}
}


#endif //__DR_UNIVERSUM_LIB_CONTROLLER_GPU_SCHEDULER_H