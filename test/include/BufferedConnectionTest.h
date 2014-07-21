#ifndef __UNIVERSUM_LIB_TEST_BUFFERED_CONNECTION_TEST__
#define __UNIVERSUM_LIB_TEST_BUFFERED_CONNECTION_TEST__

namespace UniversumLibTest {

	class BufferedConnectionTest: public Test
	{
	public:
		BufferedConnectionTest();
		~BufferedConnectionTest();

		virtual DRReturn init();
		virtual DRReturn test();
		virtual const char* getName() {return "Buffered Connection Test";};
	private:
		UniLib::lib::BufferedNetworkPacket* mInput;
		UniLib::lib::BufferedNetworkPacket* mOutput;
		int			mConnectionNumber;
	};


};

#endif //__UNIVERSUM_LIB_TEST_BUFFERED_CONNECTION_TEST__