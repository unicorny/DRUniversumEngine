#include "controller/CPUTask.h"
#include "controller/CPUSheduler.h"

namespace UniLib {
	namespace controller {
		CPUTask::CPUTask(CPUSheduler* cpuScheduler, size_t taskDependenceCount)
			: Task(taskDependenceCount), mScheduler(cpuScheduler)
		{
			assert(cpuScheduler);
		}

		CPUTask::CPUTask(CPUSheduler* cpuScheduler)
			: Task(), mScheduler(cpuScheduler)
		{
			assert(cpuScheduler);
		}

		CPUTask::~CPUTask()
		{

		}

		void CPUTask::scheduleTask(TaskPtr own)
		{
			assert(mTaskScheduled);
			if(!isTaskSheduled()) {
				mScheduler->sheduleTask(own);
				taskScheduled();
			}
		}
	}
}
