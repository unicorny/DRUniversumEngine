#include "controller/FileLoadingTask.h"

namespace UniLib {
	namespace controller {
		FileLoadingTask::FileLoadingTask(FileLoadingReciver* reciver, const char* fileName)
			: CPUTask(g_HarddiskScheduler), mFinishReciver(reciver), mFinished(false)
		{
			assert(reciver != NULL);
			mFileNames.push_back(fileName);
			mFilesInMemory = new DRVirtualFile*;
#ifdef _UNI_LIB_DEBUG
			setName(fileName);
#endif
		}

		FileLoadingTask::FileLoadingTask(FileLoadingReciver* reciver, std::vector<std::string> fileNames)
			: CPUTask(g_HarddiskScheduler), mFinishReciver(reciver), mFileNames(fileNames), mFinished(false)
		{
#ifdef _UNI_LIB_DEBUG
			setName(fileNames[0].data());
#endif
			assert(reciver != NULL);
			mFilesInMemory = new DRVirtualFile*[mFileNames.size()];
			memset(mFilesInMemory, 0, sizeof(DRVirtualFile*)*mFileNames.size());
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
					char start[11]; memset(start, 0, 11);
					if (file.getSize() > 10) {
						file.read(start, 1, 10);
						file.close();
						if (strcmp(start, VIRTUAL_FILE_HEAD) == 0) {
							mFilesInMemory[i] = new DRVirtualCustomFile();
							mFilesInMemory[i]->readFromFile(mFileNames[i].data());
							continue;
						} 
					} else {
						file.close();
					}
					mFilesInMemory[i] = new DRVirtualBinaryFile();
					
					mFilesInMemory[i]->readFromFile(mFileNames[i].data());
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
					mFilesInMemory[i] = NULL;
				}
			}
			lock();
			mFinished = true;
			unlock();
			mFinishReciver->finishFileLoadingTask();
			
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