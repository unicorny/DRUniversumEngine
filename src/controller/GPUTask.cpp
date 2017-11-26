#include "controller/GPUTask.h"
#include "controller/GPUScheduler.h"

namespace UniLib {
	namespace controller {
		GPUTask::GPUTask(GPUTaskSpeed taskSpeed/* = GPU_TASK_FAST*/)
			:Task(0), mTaskSpeed(taskSpeed)
		{

		}
		GPUTask::GPUTask(size_t childCount, GPUTaskSpeed taskSpeed /*= GPU_TASK_FAST*/)
			:Task(childCount), mTaskSpeed(taskSpeed)
		{

		}

		GPUTask::~GPUTask()
		{

		}

		void GPUTask::scheduleTask(TaskPtr own) {
			GPUScheduler::getInstance()->addGPUTask(own, mTaskSpeed);
		}
	}
}