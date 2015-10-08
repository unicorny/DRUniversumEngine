#include "controller/CPUSheduler.h"
#include "controller/CPUTask.h"
#include "controller/CPUShedulerThread.h"

namespace UniLib {
	namespace controller {

		CPUSheduler::CPUSheduler(u8 threadCount, const char* name)
			: mThreads(new CPUShedulerThread*[threadCount]), mThreadCount(threadCount), mName(name)
		{
			char nameBuffer[10]; memset(nameBuffer, 0, 10);
			u8 len = min(strlen(name), 7);
			memcpy(nameBuffer, name, len);
			for(int i = 0; i < threadCount; i++) {
				sprintf(&nameBuffer[len], "%.2d", i); 
				mThreads[i] = new CPUShedulerThread(this, nameBuffer);
			}
		}

		CPUSheduler::~CPUSheduler()
		{
			for(int i = 0; i < mThreadCount; i++) {
				DR_SAVE_DELETE(mThreads[i]);
			}
			DR_SAVE_DELETE_ARRAY(mThreads);
			mThreadCount = 0;
		}

		DRReturn CPUSheduler::sheduleTask(TaskPtr task)
		{
			CPUShedulerThread* t = NULL;
			// look at free worker threads
			if(mFreeWorkerThreads.pop(t)) {
				// gave him the new task
				t->setNewTask(task);
			} else {
				// else put task to pending queue
				mPendingTasks.push(task);
			}
			return DR_OK;
		}
		TaskPtr CPUSheduler::ImReadyForTheNextTask(CPUShedulerThread* Me)
		{
			
			// look at pending tasks
			TaskPtr task;
			if(mPendingTasks.pop(task)) {
				// return task
				return task;
			} else {
				// push thread to worker queue
				mFreeWorkerThreads.push(Me);
			}
			return TaskPtr();
		}

	}
}