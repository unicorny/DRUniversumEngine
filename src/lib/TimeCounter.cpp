#include "lib/TimeCounter.h"



namespace UniLib {
	namespace lib {
		double TimeCounter::millis() const
		{
#ifdef _WINDOWS_
			LARGE_INTEGER ll;
			QueryPerformanceCounter(&ll);
			return (double)(ll.QuadPart - mCounter.QuadPart) / (double)(g_QueryPerformanceFreq.QuadPart / 1000.0);
#else
			return (double)(SDL_GetTicks() - mCounter);
#endif 

		}
		double TimeCounter::micros() const
		{
#ifdef _WINDOWS_ 
			LARGE_INTEGER ll;
			QueryPerformanceCounter(&ll);
			return (double)(ll.QuadPart - mCounter.QuadPart) / ((double)g_QueryPerformanceFreq.QuadPart / 1000000.0);
#else
			return (double)(SDL_GetTicks() - mCounter) * 1000.0;
#endif 
		}

		std::string TimeCounter::string() const
		{
#ifdef _WINDOWS_ 
			LARGE_INTEGER ll;
			QueryPerformanceCounter(&ll);
			double v = (double)(ll.QuadPart - mCounter.QuadPart) / ((double)g_QueryPerformanceFreq.QuadPart / 1000.0);
			std::stringstream ss;
			ss.precision(3);
			if (v > 1000.0) {
				ss << (v / 1000.0) << " s";
			} else if (v > 0.25) {
				ss << v << " ms";
			} else {
				ss << (v * 1000.0) << " µs";
			}
			return ss.str();
#else
			return std::string(itoa(SDL_GetTicks() - mCounter)) + std::string(" ms");
#endif 
		}
	}
}