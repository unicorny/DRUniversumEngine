#ifndef __UNIVERSUM_LIB_TEST_CPU_TASK_TEST__
#define __UNIVERSUM_LIB_TEST_CPU_TASK_TEST__

#include "Test.h"

namespace UniLib {
	namespace controller {
		class CPUSheduler;
	}
}

namespace UniversumLibTest {

	class CPUTaskTest: public Test
	{
	public:
		CPUTaskTest();
		~CPUTaskTest();

		virtual DRReturn init();
		virtual DRReturn test();
		virtual const char* getName() {return "CPU Task Test";};
	private:
		UniLib::controller::CPUSheduler* mScheduler;
	};


};

#endif //__UNIVERSUM_LIB_TEST_CPU_TASK_TEST__