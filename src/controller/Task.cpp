#include "controller/Task.h"

namespace UniLib {
	namespace controller {
		Task::Task() 
		: mTaskScheduled(false), mParentTaskPtrArray(NULL), mParentTaskPtrArraySize(0)
		{
		}

        Task::Task(size_t taskPointerArraySize)
            : mTaskScheduled(false), mParentTaskPtrArray(new TaskPtr[taskPointerArraySize]), mParentTaskPtrArraySize(taskPointerArraySize)
        {
        }
		
		Task::~Task()
		{
            DR_SAVE_DELETE_ARRAY(mParentTaskPtrArray);
            mParentTaskPtrArraySize = NULL;
		}

        bool Task::isAllParentsReady()
        {
            for(size_t i = 0; i < mParentTaskPtrArraySize; i++) {

            }
            return true;
        }
	}
}