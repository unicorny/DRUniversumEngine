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
			TestTimer(int number, Uint32 delay, int loopCount, TimerTest* parent);

		private:
			virtual UniLib::lib::TimerReturn callFromTimer();
			int mNumber;
			int mCallCount;
			int mLoopCount;
			Uint32 mTimeByLastCall;
			Uint32 mDelay;
			TimerTest* mParent;
		};

		struct TestTimerCheck {
			TestTimerCheck() : currentlyCalledLoops(0), removedByLoopCount(0) {}

			int loopCount;
			int currentlyCalledLoops;
			int removedByLoopCount;
		};

		void loopCalled(int number);

		UniLib::lib::Timer* mTimer;
		size_t				mTestTimerCheckCount;
		TestTimerCheck*     mTestTimerChecks;
		
	};
};

#endif //__UNIVERSUM_LIB_TEST_TIMER__