#include "include\TimerTest.h"

using namespace UniLib;

namespace UniversumLibTest {

	TimerTest::TestTimer::TestTimer(int number, Uint32 delay, int loopCount, TimerTest* parent)
		: mNumber(number), mCallCount(0), mLoopCount(loopCount), mTimeByLastCall(0), mDelay(delay), mParent(parent)
	{
		//DRLog.writeToLog("init TestTimer number: %d, with delay: %d (milliseconds) and loopCount: %d",
//			number, delay, loopCount);
	}


	lib::TimerReturn TimerTest::TestTimer::callFromTimer()
	{
	//	DRLog.writeToLog("call from Timer number: %d after milliseconds: %d", mNumber, SDL_GetTicks());
		if(mTimeByLastCall > 0) {
			Uint32 diff = SDL_GetTicks() - mTimeByLastCall;
			if(diff > mDelay * 1.2 || diff < mDelay * 0.8) {
				DRLog.writeToLog("<font color='orange'>Warning: timer delay: %d, called after %d milliseconds, TestTimer number: %d</font>", mDelay, diff, mNumber);
			}
		}
		mTimeByLastCall = SDL_GetTicks();
		mParent->loopCalled(mNumber);
		return lib::GO_ON;
	}

	TimerTest::TimerTest()
		:mTimer(NULL), mTestTimerCheckCount(0), mTestTimerChecks(NULL)
	{
	}

	TimerTest::~TimerTest() 
	{
		DR_SAVE_DELETE(mTimer);
		DR_SAVE_DELETE_ARRAY(mTestTimerChecks);
	}

	// ---------------------------------------------
	DRReturn TimerTest::init() 
	{
		mTimer = new lib::Timer();
		int timerNumber = 0;
		mTimer->addTimer("1", DRResourcePtr<lib::TimerCallback>(dynamic_cast<lib::TimerCallback*>(new TestTimer(timerNumber++, 100, -1, this))), 100, -1);
		mTimer->addTimer("2", DRResourcePtr<lib::TimerCallback>(dynamic_cast<lib::TimerCallback*>(new TestTimer(timerNumber++, 200, 2, this))), 200, 2);
		mTimer->addTimer("3", DRResourcePtr<lib::TimerCallback>(dynamic_cast<lib::TimerCallback*>(new TestTimer(timerNumber++, 250, 1, this))), 200, 2);
		mTimer->addTimer("4", DRResourcePtr<lib::TimerCallback>(dynamic_cast<lib::TimerCallback*>(new TestTimer(timerNumber++, 300, 0, this))), 300, 0);

		mTestTimerCheckCount = timerNumber;
		mTestTimerChecks = new TestTimerCheck[mTestTimerCheckCount];
		mTestTimerChecks[0].loopCount = -1;
		mTestTimerChecks[1].loopCount = 2;
		mTestTimerChecks[2].loopCount = 1;
		mTestTimerChecks[3].loopCount = 0;

		return DR_OK;
	}
	void TimerTest::loopCalled(int number)
	{
		if(number < mTestTimerCheckCount) {
			mTestTimerChecks[number].currentlyCalledLoops++;
		}
	}

	DRReturn TimerTest::test()
	{
		Uint32 startTicks = SDL_GetTicks();
		while(SDL_GetTicks() - startTicks < 2000)
		{
			mTimer->move(0.0f);
			SDL_Delay(1);
		}
		mTimer->removeTimer("1");
		mTestTimerChecks[0].removedByLoopCount = mTestTimerChecks[0].currentlyCalledLoops;
		while(SDL_GetTicks() - startTicks < 1000)
		{
			mTimer->move(0.0f);
			SDL_Delay(1);
		}
		for(int i = 0; i < mTestTimerCheckCount; i++) 
		{
			TestTimerCheck ttc = mTestTimerChecks[i];
			if(ttc.loopCount == -1) continue;
			if((ttc.loopCount == 0 && ttc.currentlyCalledLoops != 1) &&
				ttc.loopCount < ttc.currentlyCalledLoops) 
			{
				DRLog.writeToLog("<font color='red'>timer number: %d was called %d times, but loop count is: %d</font>", i, ttc.currentlyCalledLoops, ttc.loopCount);
				return DR_ERROR;
			}
			if(ttc.removedByLoopCount && ttc.removedByLoopCount < ttc.currentlyCalledLoops) 
			{
				DRLog.writeToLog("<font color='red'>timer number: %d was called %d times, after removing after %d calls</font>", i, ttc.currentlyCalledLoops, ttc.removedByLoopCount);
				return DR_ERROR;
			}

		}
		
		return DR_OK;
	}



};