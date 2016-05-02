#include "controller/GPUScheduler.h"
#include "controller/Task.h"
#include "debug/CPUSchedulerTasksLog.h"

namespace UniLib {
	namespace controller {

		GPUScheduler* GPUScheduler::mpInstanz = NULL;

		GPUScheduler::GPUScheduler()
			: mThreadRunning(false), mMutex(SDL_CreateMutex()), mFrameTimeMutex(SDL_CreateMutex()), mLastFrameDurationCursor(0)
		{
			memset(mLastFrameDurations, 16, sizeof(int)*GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT);
			if(!mMutex) LOG_WARNING_SDL();  
			if(!mFrameTimeMutex) LOG_WARNING_SDL();
		}
		

		GPUScheduler::~GPUScheduler() 
		{
			stopThread();
			SDL_DestroyMutex(mMutex);
			SDL_DestroyMutex(mFrameTimeMutex);
		}

		GPUScheduler* const GPUScheduler::getInstance()
		{
			if(!mpInstanz) {
				mpInstanz = new GPUScheduler;
			}

			return (GPUScheduler*)mpInstanz;
		}

		void GPUScheduler::startThread(const char* name /* = "UniGPUSch" */)
		{

			mThreadRunning = true;
#if SDL_VERSION_ATLEAST(1,3,0)
			mThread = SDL_CreateThread(run, name, this);
#else
			mThread = SDL_CreateThread(run, this);
#endif
			mLastUpdateTicks = SDL_GetTicks();
		}

		void GPUScheduler::registerGPURenderCommand(GPURenderCall* renderCall, GPUSchedulerCommandType type)
		{
			lock();
			mGPURenderCommands[type].push_back(renderCall);
			unlock();
		}
		void GPUScheduler::unregisterGPURenderCommand(GPURenderCall* renderCall, GPUSchedulerCommandType type)
		{
			lock();
			mGPURenderCommands[type].remove(renderCall);	
			unlock();
		}
		void GPUScheduler::addGPUTask(TaskPtr task, bool slow/* = true*/)
		{
			lock();
			if(!task->isGPUTask()) LOG_ERROR_VOID("given task isn't a GPU Task");
			if(slow == true) {
				mSlowGPUTasks.push(task);
			} else {
				mFastGPUTasks.push(task);
			}
			unlock();
		}
		int GPUScheduler::run(void* data)
		{
			GPUScheduler* s = (GPUScheduler*)data;
			while(true) {
				s->lock();
				if(s->mThreadRunning == false) {
					s->unlock();
					return 0;
				}
				s->unlock();
				if(s->updateEveryRendering()) {
					s->stopThread();
					LOG_ERROR("error in main game rendering loop", DR_ERROR);
				}
			}
			return 0;
		}
		void GPUScheduler::runSlowGPUTask()
		{
#ifdef _UNI_LIB_DEBUG
			debug::CPUShedulerTasksLog* l = debug::CPUShedulerTasksLog::getInstance();
			std::string runningTasksTable;
#endif
			Uint32 ticks = SDL_GetTicks();
			if (mSlowGPUTasks.size()) {
				TaskPtr task = mSlowGPUTasks.front();
				if (task.getResourcePtrHolder() && task->isReady()) {
					mSlowGPUTasks.pop();

#ifdef _UNI_LIB_DEBUG
					l->addTaskLogEntry((HASH)task.getResourcePtrHolder(), task->getResourceType(), "GPU slow", task->getName());
					runningTasksTable = l->getCurrentlRunningTasksTableString();
#endif
					unlock();
					task->run();
					lock();
#ifdef _UNI_LIB_DEBUG
					l->removeTaskLogEntry((HASH)task.getResourcePtrHolder());
					Uint32 diff = SDL_GetTicks() - ticks;
					SpeedLog.writeToLog("%3d ms used on GPU (slow) by Task: %s of: %s",
						diff, task->getResourceType(), task->getName());
#endif
				}

				if (SDL_GetTicks() - ticks > 10) {
#ifdef _UNI_LIB_DEBUG
					EngineLog.writeToLogDirect(runningTasksTable);
					runningTasksTable = "";
#endif
					EngineLog.writeToLog("<font color='red'>used %d ms</font>", SDL_GetTicks() - ticks);
					LOG_WARNING("slow GPU Task used more then 10 ms");
				}
			}
		}
		void GPUScheduler::runFastGPUTasks()
		{
			Uint32 reQueueCount = 0;
			Uint32 startTicks = SDL_GetTicks();
#ifdef _UNI_LIB_DEBUG
			debug::CPUShedulerTasksLog* l = debug::CPUShedulerTasksLog::getInstance();
			std::string runningTasksTable;
#endif

			while (mFastGPUTasks.size() - reQueueCount) {
				TaskPtr task = mFastGPUTasks.front();
				mFastGPUTasks.pop();
				if (!task.getResourcePtrHolder()) continue;
				if (task->isReady()) {
#ifdef _UNI_LIB_DEBUG
					l->addTaskLogEntry((HASH)task.getResourcePtrHolder(), task->getResourceType(), "GPU fast", task->getName());
					runningTasksTable = l->getCurrentlRunningTasksTableString();
					Uint32 ticks = SDL_GetTicks();
#endif
					unlock();
					task->run();
					lock();
#ifdef _UNI_LIB_DEBUG
					Uint32 diff = SDL_GetTicks() - ticks;
					SpeedLog.writeToLog("%3d ms used on GPU (fast) by Task: %s of: %s",
						diff, task->getResourceType(), task->getName());
					l->removeTaskLogEntry((HASH)task.getResourcePtrHolder());
					
#endif
				}
				else {
					mFastGPUTasks.push(task);
					reQueueCount++;
				}
				if (SDL_GetTicks() - startTicks > 2) {
#ifdef _UNI_LIB_DEBUG
					//l->printCurrentlyRunningTasks();
					EngineLog.writeToLogDirect(runningTasksTable);
					runningTasksTable = "";
#endif
					EngineLog.writeToLog("<font color='red'>used %d ms</font>", SDL_GetTicks() - startTicks);
					LOG_WARNING("break fast GPU Tasks loop, has more than 2 ms used");
					break;
				}
			}
		}
		DRReturn GPUScheduler::updateEveryRendering()
		{
			
			float secondsSinceLastFrame = 0.0f;
			Uint32 sumFrames = 0;
			for(int i = 0; i < GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT; i++)
				sumFrames += mLastFrameDurations[i];
			secondsSinceLastFrame = (float)(sumFrames/GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT)/1000.0f;
			SDL_LockMutex(mFrameTimeMutex);
			mSecondsSinceLastFrame = secondsSinceLastFrame;
			SDL_UnlockMutex(mFrameTimeMutex);

			Uint32 startTicks = SDL_GetTicks();

			lock();
			// update fast GPU Tasks
			runFastGPUTasks();

			// update one slow GPU Task
			runSlowGPUTask();
	
#ifdef _UNI_LIB_DEBUG
			debug::CPUShedulerTasksLog* l = debug::CPUShedulerTasksLog::getInstance();
#endif
			// update render calls
			for(int i = 0; i < GPU_SCHEDULER_COMMAND_MAX; i++) {
				for(std::list<GPURenderCall*>::iterator it = mGPURenderCommands[i].begin(); it != mGPURenderCommands[i].end(); it++) {
					Uint32 ticks = SDL_GetTicks();
#ifdef _UNI_LIB_DEBUG
					l->addTaskLogEntry(1, getGPUCommandTypeString((GPUSchedulerCommandType)i), "GPU Render Call", (*it)->getName());
#endif 
					DRReturn result = (*it)->render(secondsSinceLastFrame);
#ifdef _UNI_LIB_DEBUG
					l->removeTaskLogEntry(1);
#endif
					Uint32 diff = SDL_GetTicks()-ticks;
					if(result) {
						(*it)->kicked();
						it = mGPURenderCommands[i].erase(it);
						if(it == mGPURenderCommands[i].end()) break;
					} else {
						if((diff > 1 && i != GPU_SCHEDULER_COMMAND_RENDERING) ||
							diff > 5 && i == GPU_SCHEDULER_COMMAND_RENDERING) {
							(*it)->youNeedToLong(((float)diff)/secondsSinceLastFrame);
						}
					}
				}
			}

			// if we have time left, let's run some more slow gpu tasks
			while(SDL_GetTicks() - startTicks < 15 && mSlowGPUTasks.size()) {
				runSlowGPUTask();
			}
			unlock();
			// if we have still time left, we wait
			if(SDL_GetTicks() - startTicks < 16) {
				SDL_Delay(16 -  SDL_GetTicks() + startTicks);
			}

			mLastFrameDurations[mLastFrameDurationCursor++] = SDL_GetTicks()- mLastUpdateTicks;
			if(mLastFrameDurationCursor >= GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT) mLastFrameDurationCursor = 0;
			mLastUpdateTicks = SDL_GetTicks();
			return DR_OK;
		}

		void GPUScheduler::stopThread()
		{
			lock();
			mThreadRunning = false;
			unlock();
		}

		const char* GPUScheduler::getGPUCommandTypeString(GPUSchedulerCommandType type)
		{
			switch (type) {
			case GPU_SCHEDULER_COMMAND_AFTER_AFTER_RENDERING: return "after after rendering";
			case GPU_SCHEDULER_COMMAND_AFTER_RENDERING: return "after rendering";
			case GPU_SCHEDULER_COMMAND_BEFORE_PREPARE_RENDERING: return "before prepare rendering";
			case GPU_SCHEDULER_COMMAND_PREPARE_RENDERING: return "prepare rendering";
			case GPU_SCHEDULER_COMMAND_RENDERING: return "rendering";
			case GPU_SCHEDULER_COMMAND_MAX: return "max";
			}
		}
	}
}