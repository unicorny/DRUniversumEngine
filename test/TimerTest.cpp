#include "include\TimerTest.h"

using namespace UniLib;

namespace UniversumLibTest {

	TimerTest::TestTimer::TestTimer(int number, Uint32 delay, int loopCount)
		: mNumber(number) 
	{
		DRLog.writeToLog("init TestTimer number: %d, with delay: %d (milliseconds) and loopCount: %d",
			number, delay, loopCount);
	}

	lib::TimerReturn TimerTest::TestTimer::callFromTimer()
	{
		DRLog.writeToLog("call from Timer number: %d after milliseconds: %d", mNumber, SDL_GetTicks());
		return lib::GO_ON;
	}

	TimerTest::TimerTest()
		:mTimer(NULL)
	{
	}

	TimerTest::~TimerTest() 
	{
		DR_SAVE_DELETE(mTimer);
	}

	// ---------------------------------------------
	DRReturn TimerTest::init() 
	{
		mTimer = new lib::Timer();
		mTimer->addTimer(DRResourcePtr<lib::TimerCallback>(dynamic_cast<lib::TimerCallback*>(new TestTimer(0, 100, -1))), 100, -1);
		mTimer->addTimer(DRResourcePtr<lib::TimerCallback>(dynamic_cast<lib::TimerCallback*>(new TestTimer(1, 200, 2))), 200, 2);
		mTimer->addTimer(DRResourcePtr<lib::TimerCallback>(dynamic_cast<lib::TimerCallback*>(new TestTimer(2, 300, 0))), 300, 0);

		return DR_OK;
	}

	DRReturn TimerTest::test()
	{
		Uint32 startTicks = SDL_GetTicks();
		while(SDL_GetTicks() - startTicks < 2000)
		{
			mTimer->move(0.0f);
			SDL_Delay(1);
		}

		
		return DR_OK;
	}



};