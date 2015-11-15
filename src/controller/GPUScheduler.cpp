#include "controller/GPUScheduler.h"
#include "controller/Task.h"

namespace UniLib {
	namespace controller {

		GPUScheduler* GPUScheduler::mpInstanz = NULL;

		GPUScheduler::GPUScheduler()
			: mThreadRunning(false), mMutex(SDL_CreateMutex()), mLastFrameDurationCursor(0)
		{
			memset(mLastFrameDurations, 16, sizeof(int)*GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT);
			if(!mMutex) LOG_WARNING_SDL();  
		}
		

		GPUScheduler::~GPUScheduler() 
		{
			stopThread();
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
			mGPURenderCommands[type].push_back(renderCall);
		}
		void GPUScheduler::unregisterGPURenderCommand(GPURenderCall* renderCall, GPUSchedulerCommandType type)
		{
			mGPURenderCommands[type].remove(renderCall);	
		}
		void GPUScheduler::addGPUTask(TaskPtr task, bool slow/* = true*/)
		{
			if(!task->isGPUTask()) LOG_ERROR_VOID("given task isn't a GPU Task");
			if(slow == true) {
				mSlowGPUTasks.push(task);
			} else {
				mFastGPUTasks.push(task);
			}
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
				if(s->updateEveryRendering()) {
					s->unlock();
					s->stopThread();
					LOG_ERROR("error in main game rendering loop", DR_ERROR);
				}
				s->unlock();
			}
			return 0;
		}
		DRReturn GPUScheduler::updateEveryRendering()
		{
			float secondsSinceLastFrame = 0.0f;
			Uint32 sumFrames = 0;
			for(int i = 0; i < GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT; i++)
				sumFrames += mLastFrameDurations[i];
			secondsSinceLastFrame = (float)(sumFrames/GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT)/1000.0f;

			Uint32 startTicks = SDL_GetTicks();
			// update fast GPU Tasks
			while(mFastGPUTasks.size()) {
				TaskPtr task = mFastGPUTasks.front();
				mFastGPUTasks.pop();
				task->run();
				if(SDL_GetTicks() - startTicks > 2) {
					LOG_WARNING("break fast GPU Tasks loop");
					break;
				}
			}
			Uint32 ticks = SDL_GetTicks();
			// update one slow GPU Task
			if(mSlowGPUTasks.size()) {
				TaskPtr task = mSlowGPUTasks.front();
				mSlowGPUTasks.pop();
				task->run();
				if(SDL_GetTicks() - ticks > 5) {
					LOG_WARNING("slow GPU Task used more then 5 ms");
				}
			}
			
			// update render calls
			for(int i = 0; i < GPU_SCHEDULER_COMMAND_MAX; i++) {
				for(std::list<GPURenderCall*>::iterator it = mGPURenderCommands[i].begin(); it != mGPURenderCommands[i].end(); it++) {
					ticks = SDL_GetTicks();
					DRReturn result = (*it)->render(secondsSinceLastFrame);
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
				TaskPtr task = mSlowGPUTasks.front();
				mSlowGPUTasks.pop();
				task->run();
			}

			// if we have still time left, we wait
			if(SDL_GetTicks() - startTicks < 16) {
				SDL_Delay(16 -  SDL_GetTicks() + startTicks);
			}

			mLastFrameDurations[mLastFrameDurationCursor] = SDL_GetTicks()- mLastUpdateTicks;
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
	}
}