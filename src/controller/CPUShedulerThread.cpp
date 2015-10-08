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
			mWaitingTask = mParent->ImReadyForTheNextTask(this);

		}

		CPUShedulerThread::~CPUShedulerThread()
		{
		}

		int CPUShedulerThread::ThreadFunction()
		{
			while(mWaitingTask.getResourcePtrHolder()) 
			{
				mWaitingTask->run();
				mWaitingTask = mParent->ImReadyForTheNextTask(this);
			}
			return 0;
		}

		void CPUShedulerThread::setNewTask(TaskPtr cpuTask)
		{
			lock();
			mWaitingTask = cpuTask;
			unlock();
			condSignal();
		}
	}
}