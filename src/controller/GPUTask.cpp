#include "controller/GPUTask.h"
#include "controller/GPUScheduler.h"

namespace UniLib {
	namespace controller {
		GPUTask::GPUTask(bool slowGPUTask/* = false*/)
			:Task(0), mSlowTask(slowGPUTask)
		{

		}
		GPUTask::GPUTask(size_t childCount, bool slowGPUTask/* = false*/)
			:Task(childCount), mSlowTask(slowGPUTask)
		{

		}

		GPUTask::~GPUTask()
		{

		}

		void GPUTask::scheduleTask(TaskPtr own) {
			GPUScheduler::getInstance()->addGPUTask(own, mSlowTask);
		}
	}
}