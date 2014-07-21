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
		mConnectionNumber = DRINetwork::Instance()->createBufferedConnection("buffConnection", "http://localhost/spacecraft", 80, mInput, mOutput);
		return DR_OK;
	}


	DRReturn BufferedConnectionTest::test()
	{
		Uint32 startTicks = SDL_GetTicks();
		while(SDL_GetTicks() - startTicks < 2000) 
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