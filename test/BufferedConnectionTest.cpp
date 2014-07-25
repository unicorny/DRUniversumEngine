#include "include\main.h"

namespace UniversumLibTest {
	BufferedConnectionTest::BufferedConnectionTest()
		: mInput(NULL), mOutput(NULL), mConnectionNumber(0)
	{
	}

	BufferedConnectionTest::~BufferedConnectionTest()
	{
		DRINetwork::Instance()->exit();
		DR_SAVE_DELETE(mInput);
		DR_SAVE_DELETE(mOutput);
	}

	// ------------------------------------------------------------------

	DRReturn BufferedConnectionTest::init()
	{
		mInput = new UniLib::lib::BufferedNetworkPacket();
		mOutput = new UniLib::lib::BufferedNetworkPacket();
		if(DRINetwork::Instance()->init()) LOG_ERROR("error by init Network Interface", DR_ERROR);
		mConnectionNumber = DRINetwork::Instance()->createBufferedConnection("buffConnection", "127.0.0.1/spacecraft/players/login_raw", 80, mInput, mOutput);
		//DRINetwork::Instance()->HTTPRequest("127.0.0.1/spacecraft", NET_GET, "", "spaceCraft");
		return DR_OK;
	}


	DRReturn BufferedConnectionTest::test()
	{
		Uint32 startTicks = SDL_GetTicks();

		while(SDL_GetTicks() - startTicks < 4000) 
		{
			if(DRINetwork::Instance()->update(1)) LOG_ERROR("error by update Network", DR_ERROR);
			if(mInput->pushDataWrapHTTPRequest(Json::Value(), "UniLibTest", "", NET_GET)) LOG_ERROR("error by parsing HTTP Request", DR_ERROR);
			SDL_Delay(1);
			std::string out = mOutput->popDataString();
			if(out.length() > 0) {
				UniLib::EngineLog.writeToLogDirect(out);
				return DR_OK;
			}			
		}
		if(!DRINetwork::Instance()->removeBufferedConnection("buffConnection")) LOG_ERROR("no buffer removed", DR_ERROR);

		return DR_ERROR;
	}
}