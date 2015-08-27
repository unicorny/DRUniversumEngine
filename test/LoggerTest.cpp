#include "include/LoggerTest.h"

namespace UniversumLibTest {
    LoggerTest::LoggerTest()
    {

    }

    LoggerTest::~LoggerTest()
    {

    }

    DRReturn LoggerTest::init()
    {
        return DR_OK;
    }

    DRReturn LoggerTest::test()
    {
        // test u8
        u8 testZahl = 1 + 4 + 128;
        DRString binaryString("10100001\0");
        DRString compareString = UniLib::getValueAsBinaryString(testZahl);
        if(binaryString != compareString) return DR_ERROR;

        UniLib::EngineLog.writeToLog("please compare, auto test not possible");

        // test u16
        u16 secondTestZahl = 2 + 8 + 256 + 1024;
        binaryString = DRString("01010000 10100000 == ");
        UniLib::EngineLog.writeAsBinary(binaryString, secondTestZahl);

        // test u32
        u32 thirdTestZahl = 4 + 16 + 1024 + 4096 + (u32)pow(2.0, 18) + (u32)pow(2.0, 24) + (u32)pow(2.0, 30);
        binaryString = DRString("00101000 00101000 00100000 10000010 == ");
        UniLib::EngineLog.writeAsBinary(binaryString, thirdTestZahl);

        // test u64
        u64 fourthTestZahl = thirdTestZahl + (((u64)(thirdTestZahl)) << 32);
        binaryString = DRString("00101000 00101000 00100000 10000010 00101000 00101000 00100000 10000010 == ");
        //UniLib::EngineLog.writeToLog("hex: %x", fourthTestZahl);
        UniLib::EngineLog.writeAsBinary(binaryString, fourthTestZahl);

        return DR_OK;
    }
}
