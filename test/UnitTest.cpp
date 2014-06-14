#include "include\UnitTest.h"

namespace UniversumLibTest {
	UnitTest::UnitTest()
	{
		memset(mTestDaten, 0, sizeof(UnitTestDaten)*TEST_DATEN_COUNT);
	}
	UnitTest::~UnitTest() 
	{
	}

	DRReturn UnitTest::init()
	{
		int i = 0;
		mTestDaten[i++] = UnitTestDaten(1.0, 1, 0);
		mTestDaten[i++] = UnitTestDaten(1.1, 1, 1);
		mTestDaten[i++] = UnitTestDaten(12.1, 2, 1);
		mTestDaten[i++] = UnitTestDaten(28173.123, 5, 3);
		mTestDaten[i++] = UnitTestDaten(8439221.12313, 7, 5);
		mTestDaten[i++] = UnitTestDaten(7463.1112428, 4, 7);
		mTestDaten[i++] = UnitTestDaten(0.2463728392019, 0, 13);
		mTestDaten[i++] = UnitTestDaten(16781938271.82917313, 11, 8);
		return DR_OK;
	}

	DRReturn UnitTest::test()
	{
		for(int i = 0; i < 8; i++) 
		{
			double value = mTestDaten[i];
			double beforeComma = floor(value);
			double afterComma = value - beforeComma;
			int log = ceil(log10(beforeComma));
			if(log == 0 && beforeComma > 0) log = 1;
			DRLog.writeToLog("value: %f, before comma: %f, after comma: %f, count: %d\n",
				value, beforeComma, afterComma, log);
		}
		return DR_OK;
	}
};