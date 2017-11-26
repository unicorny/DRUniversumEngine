#include "controller/Task.h"


namespace UniLib {
	namespace controller {
		Task::Task() 
		: mTaskScheduled(false), mFinishCommand(NULL), mParentTaskPtrArray(NULL),
		  mParentTaskPtrArraySize(0), mWorkingMutex(SDL_CreateMutex()), mDeleted(false)
		{
		}

        Task::Task(size_t taskPointerArraySize)
            : mTaskScheduled(false), mFinishCommand(NULL), mParentTaskPtrArray(new TaskPtr[taskPointerArraySize]), mParentTaskPtrArraySize(taskPointerArraySize),
            mWorkingMutex(SDL_CreateMutex()), mDeleted(false)
        {
        }
		
		Task::~Task()
		{
            DR_SAVE_DELETE_ARRAY(mParentTaskPtrArray);
            mParentTaskPtrArraySize = NULL;
            SDL_LockMutex(mWorkingMutex);
            mDeleted = true;
            SDL_UnlockMutex(mWorkingMutex);
            SDL_DestroyMutex(mWorkingMutex);
		}

        bool Task::isAllParentsReady()
        {
            bool allFinished = true;
			lock();
            for(size_t i = 0; i < mParentTaskPtrArraySize; i++) {
                TaskPtr task = mParentTaskPtrArray[i];
				if (!task.getResourcePtrHolder()) {
					allFinished = false;
					continue;
				}
                if(!task->isTaskFinished()) {
                    allFinished = false;
                    if(!task->isTaskSheduled()) 
                        mParentTaskPtrArray[i]->scheduleTask(mParentTaskPtrArray[i]);
                }
            }
			unlock();
            return allFinished;
        }

	}
}