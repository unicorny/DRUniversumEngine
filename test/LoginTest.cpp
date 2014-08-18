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
		std::string request = "{\"url\":\"/spacecraft/serverKeys/get\"}";
		Json::Reader reader;
		Json::Value json;
		reader.parse(request, json);
		UniLib::EngineLog.writeToLog("url: %s", json.get("url", ""));
		DRINetwork::Instance()->send(request, mConnectionNumber);
		DRINetwork::Instance()->login("dariofrodo", "ssss");

		std::string pubKey, privateKey;
		UniLib::g_RSAModule->generateKeyPair(pubKey, privateKey);
		UniLib::EngineLog.writeToLog("generate Keys: public key: %s, private key: %s", pubKey.data(), privateKey.data());

		while(SDL_GetTicks() - startTicks < 8000) 
		{
			std::string recv;
			if(DRINetwork::Instance()->recv(recv, mConnectionNumber) == NET_COMPLETE) {
				//UniLib::EngineLog.writeToLog("connection recv: %s", recv.data());
				Json::Reader reader;
				Json::Value json;
				reader.parse(recv, json);
				if(json.empty()) 
					LOG_ERROR(reader.getFormattedErrorMessages().data(), DR_ERROR);
				std::string pub_key = json.get("public_key", "").asString();
				UniLib::EngineLog.writeToLog("public key: %s", pub_key.data());
				UniLib::g_RSAModule->crypt("h7JD83l29DK", pub_key);
				break;
			}
			SDL_Delay(100);
		}
		DRINetwork::Instance()->disconnect(mConnectionNumber);

		return DR_ERROR;
	}
}