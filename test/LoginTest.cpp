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
		
		DRINetwork::Instance()->send(request, mConnectionNumber);
//		DRINetwork::Instance()->login("dariofrodo", "ssss");

		
		//printf("publickey: %s, privateKey: %s\n", pubKey.data(), privateKey.data());
		unsigned int state = 0;
		while(SDL_GetTicks() - startTicks < 8000) 
		{
			std::string recv;
			if(DRINetwork::Instance()->recv(recv, mConnectionNumber) == NET_COMPLETE) {
				// get public key from server
				if(!state) {
					//UniLib::EngineLog.writeToLog("connection recv: %s", recv.data());
					Json::Reader reader;
					Json::Value json;
					reader.parse(recv, json);
					if(json.empty()) 
					{
						UniLib::EngineLog.writeToLog(std::string("recv: ") + recv);
						LOG_ERROR(reader.getFormattedErrorMessages().data(), DR_ERROR);
					}
					//UniLib::EngineLog.writeToLog(std::string("recv: ") + recv);
					Json::Value publicKey = json.get("public_key", "");
					std::string pub_key;
					if(publicKey.isObject()) 
					{
						pub_key = publicKey.get("key", "").asString();					
					}
					else if(publicKey.isString() )
					{
						pub_key = json.get("public_key", "").asString();
					}
					//UniLib::EngineLog.writeToLog("public key: %s", pub_key.data());
					if(UniLib::g_RSAModule->setServerPublicKey(pub_key, 3)) {
						UniLib::EngineLog.writeToLog(std::string("transfered public key: ") + pub_key);
						LOG_ERROR("cannot use transfered public key", DR_ERROR);
					}
					
					std::string encrypted = UniLib::g_RSAModule->crypt("h7JD83l29DK", UniLib::lib::Crypto::CRYPT_WITH_SERVER_PUBLIC);
					//std::string encrypted = UniLib::g_RSAModule->crypt("halloWelt", UniLib::lib::Crypto::CRYPT_WITH_SERVER_PUBLIC);
					//UniLib::EngineLog.writeToLog(std::string("client encrypted: ") + encrypted);
					Json::Value loginRequest(Json::objectValue);
					loginRequest["url"] = "/spacecraft/playersRaw/login";
					//loginRequest["url"] = "/spacecraft/players/login";
					//loginRequest["url"] = "/spacecraftLoginTest.php";
					loginRequest["method"] = "POST";
					loginRequest["contentType"] = "application/x-www-form-urlencoded";
					//loginRequest["contentType"] = "application/jsonrequest";
					//loginRequest["content"] = encrypted;
					//loginRequest["content"] = "{\"now\":\"entchen\",\"username\":\"einhornimmond\"}";
					loginRequest["content"] = std::string("json={\"username\":\"admin\",\"password\":\"") + encrypted + std::string("\"}");
					//loginRequest["content"] = std::string("json={\"username\":\"admin\",\"password\":\"") + std::string("h7JD83l29DK") + std::string("\"}");
					Json::FastWriter writer;
					DRINetwork::Instance()->send(writer.write(loginRequest), mConnectionNumber);
					state = 1;
					//login with crypted password
				} else if(state == 1) {
					UniLib::EngineLog.writeToLog(std::string("recv login answear") + recv);
				}
			}
			SDL_Delay(100);
		}
		DRINetwork::Instance()->disconnect(mConnectionNumber);

		return DR_ERROR;
	}
}