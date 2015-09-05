#ifndef __UNIVERSUM_LIB_TEST_LOGIN_TEST__
#define __UNIVERSUM_LIB_TEST_LOGIN_TEST__

#include "Test.h"
#include "controller/Command.h"

namespace UniLib
{
	namespace lib {
		class Crypto;
	}
	namespace controller {
		class Task;
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
		SDL_cond*	mCondition;
		SDL_mutex*  mMutex;
	};

	class LoginCommandFinish : public UniLib::controller::Command
	{
	public:
		LoginCommandFinish(SDL_cond* cond, SDL_mutex* mutex);
		virtual DRReturn taskFinished(UniLib::controller::Task* task);
	protected:
		SDL_cond* mCond;
		SDL_mutex* mMutex;
	};


};

#endif //__UNIVERSUM_LIB_TEST_LOGIN_TEST__