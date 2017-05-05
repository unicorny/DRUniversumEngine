#include "controller/FileSavingTask.h"

namespace UniLib {
	namespace controller {

		FileSavingTask::FileSavingTask(const char* fileName, DRVirtualFile* data, bool freeMemory /* = true */)
			: CPUTask(g_HarddiskScheduler), mFileName(fileName), mData(data), mFreeMemory(freeMemory)
		{
#ifdef _UNI_LIB_DEBUG
			setName(fileName);
#endif
		}

		FileSavingTask::~FileSavingTask() 
		{
			if (mFreeMemory) {
				DR_SAVE_DELETE(mData);
			}
		}

		DRReturn FileSavingTask::run() 
		{
			if (mData->saveToFile(mFileName.data())) {
				LOG_ERROR("error saving data to file", DR_ERROR);
			}
			return DR_OK;
		}
	}
}