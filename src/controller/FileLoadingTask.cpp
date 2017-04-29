#include "controller/FileLoadingTask.h"

namespace UniLib {
	namespace controller {
		FileLoadingTask::FileLoadingTask(FileLoadingReciver* reciver, const char* fileName)
			: CPUTask(g_HarddiskScheduler), mFinishReciver(reciver), mFinished(false)
		{
			assert(reciver != NULL);
			mFileNames.push_back(fileName);
			mFilesInMemory = new FileInMemory*;
		}

		FileLoadingTask::FileLoadingTask(FileLoadingReciver* reciver, std::vector<std::string> fileNames)
			: CPUTask(g_HarddiskScheduler), mFinishReciver(reciver), mFileNames(fileNames), mFinished(false)
		{
			assert(reciver != NULL);
			mFilesInMemory = new FileInMemory*[mFileNames.size()];
			memset(mFilesInMemory, 0, sizeof(FileInMemory*)*mFileNames.size());
		}
		FileLoadingTask::~FileLoadingTask()
		{
			if (mFileNames.size() > 1) {
				DR_SAVE_DELETE_ARRAY(mFilesInMemory);
			}
			else { 
				DR_SAVE_DELETE(mFilesInMemory);
			}
			mFileNames.clear();
		}

		DRReturn FileLoadingTask::run()
		{
			
			for (u16 i = 0; i < mFileNames.size(); i++) {
				DRFile file(mFileNames[i].data(), "rb");
				if (file.isOpen()) {
					mFilesInMemory[i] = new FileInMemory(file.getSize());
					file.read(mFilesInMemory[i]->data, mFilesInMemory[i]->size, 1);
					file.close();
				}
				//mData[i] = (void*)malloc()
			}
			bool result = false;
			if (mFileNames.size() == 1) {
				result = mFinishReciver->getFileFromMemory(*mFilesInMemory);
			}
			else {
				result = mFinishReciver->getFileFromMemory(mFilesInMemory, mFileNames.size());
			}
			if (result != true) {
				for (u16 i = 0; i < mFileNames.size(); i++) {
					if (!mFilesInMemory[i]) continue;
					mFilesInMemory[i]->data = NULL;
					mFilesInMemory[i]->size = 0;
				}
			}
			lock();
			mFinished = true;
			unlock();
			
			return DR_OK;
		}

		bool FileLoadingTask::isTaskFinished()
		{
			bool state = false;
			lock();
			state = mFinished;
			unlock();
			return state;
		}
	}

	
}