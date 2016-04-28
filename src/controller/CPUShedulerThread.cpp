#include "controller/CPUShedulerThread.h"
#include "controller/CPUSheduler.h"
#include "controller/Task.h"

namespace UniLib {
	namespace controller {
		CPUShedulerThread::CPUShedulerThread(CPUSheduler* parent, const char* name)
			: Thread(name), mParent(parent)
		{
#ifdef _UNI_LIB_DEBUG
			mName = name;
#endif
			mWaitingTask = mParent->getNextUndoneTask(this);

		}

		CPUShedulerThread::~CPUShedulerThread()
		{
		}

		int CPUShedulerThread::ThreadFunction()
		{
			while(mWaitingTask.getResourcePtrHolder()) 
			{
				EngineLog.writeToLog("<font color='orange'>start task: %s</font>", mWaitingTask->getResourceType());
				mWaitingTask->run();
				EngineLog.writeToLog("<font color='orange'>end task: %s</font>", mWaitingTask->getResourceType());
				mWaitingTask = mParent->getNextUndoneTask(this);
			}
			return 0;
		}

		void CPUShedulerThread::setNewTask(TaskPtr cpuTask)
		{
			threadLock();
			mWaitingTask = cpuTask;
			threadUnlock();
			condSignal();
		}
	}
}