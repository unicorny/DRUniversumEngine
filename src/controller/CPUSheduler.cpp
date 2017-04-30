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
			if(task->isAllParentsReady() && mFreeWorkerThreads.pop(t)) {
				// gave him the new task
				t->setNewTask(task);
			} else {
				// else put task to pending queue
				mPendingTasksMutex.lock();
				mPendingTasks.push_back(task);
				mPendingTasksMutex.unlock();
			}
			return DR_OK;
		}
		TaskPtr CPUSheduler::getNextUndoneTask(CPUShedulerThread* Me)
		{
			// look at pending tasks
			TaskPtr task;
			mPendingTasksMutex.lock();
			for (std::list<TaskPtr>::iterator it = mPendingTasks.begin(); it != mPendingTasks.end(); it++) {
				if ((*it)->isAllParentsReady()) {
					task = *it;
					mPendingTasks.erase(it);
					mPendingTasksMutex.unlock();
					return task;
				}
			}
			mPendingTasksMutex.unlock();
			// push thread to worker queue
			if (Me) {
				mFreeWorkerThreads.push(Me);
			}
			
			return TaskPtr();
		}
		void CPUSheduler::checkPendingTasks()
		{
			TaskPtr task = getNextUndoneTask(NULL);
			if (task.getResourcePtrHolder()) {
				sheduleTask(task);
			}
		}

	}
}