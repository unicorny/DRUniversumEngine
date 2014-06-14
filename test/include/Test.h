#ifndef __UNIVERSUM_LIB_TEST__
#define __UNIVERSUM_LIB_TEST__

#include "../include/UniversumLib.h"

namespace UniversumLibTest {
	class Test 
	{
	public:
		virtual DRReturn init() = 0;
		virtual DRReturn test() = 0;
		virtual const char* getName() = 0;
	};
};

#endif //__UNIVERSUM_LIB_TEST__