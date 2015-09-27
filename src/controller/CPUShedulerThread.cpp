#include "controller/CPUShedulerThread.h"

namespace UniLib {
	namespace controller {
		CPUShedulerThread::CPUShedulerThread(const char* name)
			: Thread(name)
		{
#ifdef _UNI_LIB_DEBUG
			mName = name;
#endif
		}

		CPUShedulerThread::~CPUShedulerThread()
		{

		}

		int CPUShedulerThread::ThreadFunction()
		{
			return 0;
		}
	}
}