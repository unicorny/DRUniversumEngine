
#include "include/CPUTaskTest.h"
//#include "controller/CPUTask.h"
#include "controller/CPUSheduler.h"
#include "controller/CPUShedulerThread.h"

using namespace UniLib;

namespace UniversumLibTest {
	CPUTaskTest::CPUTaskTest(): mWorkMutex(SDL_CreateMutex()), mFinishedTasks(0), mFinishedCountNotNumber(false)
	{

	}

	CPUTaskTest::~CPUTaskTest()
	{
		DR_SAVE_DELETE(mScheduler);
		DR_SAVE_DELETE(mWorkMutex);
	}

	DRReturn CPUTaskTest::init()
	{
		//mScheduler = new controller::CPUSheduler(12, "testShd");
		mScheduler = new controller::CPUSheduler(12, "testShd");
		u8 count;
#ifdef _UNI_LIB_DEBUG
		controller::CPUShedulerThread** threads = mScheduler->getThreads(count);
		if(count != 12) {
			EngineLog.writeToLog("count: %d (%d)", count, 12);
			LOG_ERROR("error, Sheduler hasn't createt all threads", DR_ERROR);
		}
		char name[] = "testShd00\0";
		for(int i = 0; i < count; i++) {
			sprintf(&name[7], "%.2d", i);
			if(threads[i]->getName() != std::string(name)) {
				EngineLog.writeToLog("name: %s (%s)",
					threads[i]->getName().data(), name);
				LOG_ERROR("thread has wrong name", DR_ERROR);
			}
		}
#endif
		return DR_OK;
	}
	DRReturn CPUTaskTest::test()
	{
		for(int i = 0; i < 48; i++) {
			controller::CPUTask* cpuTask = new HeavyWorkTask(mScheduler, i);
			controller::TaskPtr task(cpuTask);
			cpuTask->setFinishCommand(new CPUThreadFinishCommand(this));
			cpuTask->start(task);
		}
		Uint32 startTicks = SDL_GetTicks();
		while(SDL_GetTicks() - startTicks < 10000) {
			SDL_Delay(1);
			SDL_LockMutex(mWorkMutex);
			if(mFinishedTasks == 48) break;
			SDL_UnlockMutex(mWorkMutex);
		}
		if(mFinishedCountNotNumber) return DR_OK;
		LOG_ERROR("tasks runned after one another, no threading or only one thread?", DR_ERROR);
	}
	void CPUTaskTest::finishTask(int number, Uint32 startTicks)
	{
		SDL_LockMutex(mWorkMutex);
		if(number != mFinishedTasks) mFinishedCountNotNumber = true;
		mFinishedTasks++;
		EngineLog.writeToLog("finish task %d with %d ms", number, SDL_GetTicks() - startTicks);
		SDL_UnlockMutex(mWorkMutex);
	}
	//

}

