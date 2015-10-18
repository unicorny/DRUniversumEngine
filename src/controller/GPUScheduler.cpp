#include "controller/GPUScheduler.h"

namespace UniLib {
	namespace controller {
		GPUScheduler::GPUScheduler()
			: mThreadRunning(true), mMutex(SDL_CreateMutex()), mLastFrameDurationCursor(0)
		{
			memset(mLastFrameDurations, 16, sizeof(int)*GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT);
			if(!mMutex) LOG_WARNING_SDL();  

#if SDL_VERSION_ATLEAST(1,3,0)
			mThread = SDL_CreateThread(run, "UniGPUSch", this);
#else
			mThread = SDL_CreateThread(run, this);
#endif
			mLastUpdateTicks = SDL_GetTicks();
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




			mLastFrameDurations[mLastFrameDurationCursor] = SDL_GetTicks()- mLastUpdateTicks;
			if(mLastFrameDurationCursor >= GPU_RENDER_LOOP_SAVED_FRAME_DURATION_COUNT) mLastFrameDurationCursor = 0;
			mLastUpdateTicks = SDL_GetTicks();
		}
		void GPUScheduler::stopThread()
		{
			lock();
			mThreadRunning = false;
			unlock();
		}
	}
}