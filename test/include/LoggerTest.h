#ifndef __UNIVERSUM_LIB_TEST_LOGGER_TEST__
#define __UNIVERSUM_LIB_TEST_LOGGER_TEST__

#include "Test.h"

namespace UniversumLibTest {

    class LoggerTest: public Test
    {
    public:
        LoggerTest();
        ~LoggerTest();

        virtual DRReturn init();
        virtual DRReturn test();
        virtual const char* getName() {return "Logger Test";};
    private:
    };


};

#endif //__UNIVERSUM_LIB_TEST_LOGGER_TEST__