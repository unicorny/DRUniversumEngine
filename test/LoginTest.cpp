#include "include\main.h"

namespace UniversumLibTest {
	LoginTest::LoginTest()
		: mConnectionNumber(0)
	{
	}

	LoginTest::~LoginTest()
	{
		DRINetwork::Instance()->exit();
	}

	// ------------------------------------------------------------------

	DRReturn LoginTest::init()
	{
		if(DRINetwork::Instance()->init()) LOG_ERROR("error by init Network Interface", DR_ERROR);
		DRFileManager::Instance().addOrdner("cfg");
		std::string value = UniLib::readFileAsString("LoginServer.json");
		mConnectionNumber = DRINetwork::Instance()->connect(value, std::string("LoginServer"));
		UniLib::EngineLog.writeToLog("connectionNumber get: %d", mConnectionNumber);
		//DRINetwork::Instance()->HTTPRequest("127.0.0.1/spacecraft", NET_GET, "", "spaceCraft");
		return DR_OK;
	}


	DRReturn LoginTest::test()
	{
		Uint32 startTicks = SDL_GetTicks();
		std::string request = "{\"url\":\"/spacecraft/publicKey/get\"}";
		Json::Reader reader;
		Json::Value json;
		reader.parse(request, json);
		UniLib::EngineLog.writeToLog("url: %s", json.get("url", ""));
		DRINetwork::Instance()->send(request, mConnectionNumber);
		DRINetwork::Instance()->login("dariofrodo", "ssss");

		while(SDL_GetTicks() - startTicks < 8000) 
		{
			std::string recv;
			if(DRINetwork::Instance()->recv(recv, mConnectionNumber) == NET_COMPLETE) {
				UniLib::EngineLog.writeToLog("connection recv: %s", recv.data());
				break;
			}
			SDL_Delay(100);
		}
		DRINetwork::Instance()->disconnect(mConnectionNumber);

		return DR_ERROR;
	}
}