
#include "include/CPUTaskTest.h"
#include "controller/CPUSheduler.h"
#include "controller/CPUShedulerThread.h"

using namespace UniLib;

namespace UniversumLibTest {
	CPUTaskTest::CPUTaskTest()
	{

	}

	CPUTaskTest::~CPUTaskTest()
	{
		DR_SAVE_DELETE(mScheduler);
	}

	DRReturn CPUTaskTest::init()
	{
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
		return DR_OK;
	}
	//

}

