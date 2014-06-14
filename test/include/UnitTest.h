#ifndef __UNIVERSUM_LIB_TEST_UNIT__
#define __UNIVERSUM_LIB_TEST_UNIT__

#include "model/Unit.h"
#include "Test.h"


namespace UniversumLibTest 
{

#define TEST_DATEN_COUNT 128
	class UnitTest : public Test
	{
	public:
		UnitTest();
		~UnitTest();
		virtual DRReturn init();
		virtual DRReturn test();
		virtual const char* getName() {return "UnitTest";}
	private:
		struct UnitTestDaten {
			
			UnitTestDaten(double _value, short _beforeComma, short _afterComma) 
			: value(_value),  beforeComma(_beforeComma), afterComma(_afterComma) {}
			UnitTestDaten()
			: value(0.0), beforeComma(0), afterComma(0) {}
			inline operator double() {return value;}

			double value;
			short beforeComma;
			short afterComma;
			
		};
		UnitTestDaten mTestDaten[TEST_DATEN_COUNT];
	};
};

#endif