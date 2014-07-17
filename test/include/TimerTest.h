#ifndef __UNIVERSUM_LIB_TEST_TIMER__
#define __UNIVERSUM_LIB_TEST_TIMER__

#include "UniversumLib.h"
#include "Test.h"

namespace UniversumLibTest {
	class TimerTest: public Test 
	{
	public:
		// constructor and deconstructor
		TimerTest();
		~TimerTest();

		virtual DRReturn init();
		virtual DRReturn test();
		virtual const char* getName() {return "TimerTest";};
	private:

		class TestTimer: public UniLib::lib::TimerCallback
		{
		public:
			TestTimer(int number, Uint32 delay, int loopCount);

		private:
			virtual UniLib::lib::TimerReturn callFromTimer();
			int mNumber;
		};

		UniLib::lib::Timer* mTimer;
		
	};
};

#endif //__UNIVERSUM_LIB_TEST_TIMER__