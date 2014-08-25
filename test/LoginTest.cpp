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
		UniLib::g_RSAModule->generateClientKeys();
		pubKey =  UniLib::g_RSAModule->getClientPublicKey();
		privateKey = UniLib::g_RSAModule->getClientPrivateKey();
		UniLib::EngineLog.writeToLog(std::string("generate Keys: public key: ") + pubKey + std::string(", privateKey: ") + privateKey);
		UniLib::EngineLog.writeToLog("<hr>");
		
		//printf("publickey: %s, privateKey: %s\n", pubKey.data(), privateKey.data());

		while(SDL_GetTicks() - startTicks < 8000) 
		{
			std::string recv;
			if(DRINetwork::Instance()->recv(recv, mConnectionNumber) == NET_COMPLETE) {
				//UniLib::EngineLog.writeToLog("connection recv: %s", recv.data());
				Json::Reader reader;
				Json::Value json;
				reader.parse(recv, json);
				if(json.empty()) 
				{
					UniLib::EngineLog.writeToLog(std::string("recv: ") + recv);
					LOG_ERROR(reader.getFormattedErrorMessages().data(), DR_ERROR);
				}
				Json::Value publicKey = json.get("public_key", "");
				std::string test = json.get("test", "").asString();
				UniLib::EngineLog.writeToLog(std::string("test: ") + test);
				if(publicKey.isObject()) 
				{
					std::string key = publicKey.get("key", "").asString();
					int type = atoi(publicKey.get("type", "").asString().c_str());
					int bits = atoi(publicKey.get("bits", "").asString().c_str());
					std::string e = publicKey.get("e", "").asString();
					std::string n = publicKey.get("n", "").asString();
					
					UniLib::EngineLog.writeToLog(std::string("key: ") + key + std::string("<br>e: ") + e + std::string(", n:") + n );
					UniLib::EngineLog.writeToLog("type: %d, bits: %d", type, bits);
					if(UniLib::g_RSAModule->setServerPublicKey(key, 3)) {
						LOG_WARNING("key isn't valid");
					} else {
						LOG_INFO("key is valid");
					}
					if(UniLib::g_RSAModule->setServerPublicKey(test, n, 3)) {
						LOG_ERROR("key e n isn't valid", DR_ERROR);
					} else {
						LOG_INFO("key e n is valid");
					}
				}
				else if(publicKey.isString() )
				{
					std::string pub_key = json.get("public_key", "").asString();
					UniLib::EngineLog.writeToLog("public key: %s", pub_key.data());
//				if(UniLib::g_RSAModule->setServerPublicKey(pub_key, 3))
	//				LOG_ERROR("key isn't valid", DR_ERROR);
				}
				UniLib::g_RSAModule->crypt("h7JD83l29DK", UniLib::lib::Crypto::CRYPT_WITH_SERVER_PUBLIC);
				break;
			}
			SDL_Delay(100);
		}
		DRINetwork::Instance()->disconnect(mConnectionNumber);

		return DR_ERROR;
	}
}