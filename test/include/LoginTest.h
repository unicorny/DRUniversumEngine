#ifndef __UNIVERSUM_LIB_TEST_LOGIN_TEST__
#define __UNIVERSUM_LIB_TEST_LOGIN_TEST__

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
		int			mConnectionNumber;
	};


};

#endif //__UNIVERSUM_LIB_TEST_LOGIN_TEST__