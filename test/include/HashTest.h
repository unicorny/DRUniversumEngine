#ifndef __UNIVERSUM_LIB_TEST_HASH_TEST__
#define __UNIVERSUM_LIB_TEST_HASH_TEST__

#include "Test.h"

namespace UniversumLibTest {

	class HashTest: public Test
	{
	public:
		HashTest();
		~HashTest();

		virtual DRReturn init();
		virtual DRReturn test();
		virtual const char* getName() {return "Hash Test";};
	private:
	};


};

#endif //__UNIVERSUM_LIB_TEST_LOGGER_TEST__