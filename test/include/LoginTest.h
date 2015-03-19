#ifndef __UNIVERSUM_LIB_TEST_LOGIN_TEST__
#define __UNIVERSUM_LIB_TEST_LOGIN_TEST__

#include "Test.h"

namespace UniLib
{
	namespace lib {
		class Crypto;
	}
}

namespace UniversumLibTest {

	class LoginTest: public Test
	{
	public:
		LoginTest();
		~LoginTest();

		virtual DRReturn init();
		virtual DRReturn test();
		virtual const char* getName() {return "Login Test";};
	private:
		int						mConnectionNumber;
		UniLib::lib::Crypto*	mRSA;
	};


};

#endif //__UNIVERSUM_LIB_TEST_LOGIN_TEST__