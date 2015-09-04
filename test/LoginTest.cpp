
#include "include/LoginTest.h"
#include "lib/DRINetwork.h"
#include "lib/Crypto.h"
#include "server/SektorConnectionManager.h"
//#include "UniversumLib.h"

namespace UniversumLibTest {
	LoginTest::LoginTest()
		
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
		DRFileManager::Instance().addOrdner("data");
        return DR_OK;
        /*UniLib::server::SektorConnectionManager::getInstance()->login();
		//std::string value = UniLib::readFileAsString("LoginServer.json");
		mRSA = DRINetwork::getSingleton().createCrypto();
        if(!mRSA) return DR_ERROR; 
		mRSA->generateClientKeys();
		DRNetServerConfig cfg;
		cfg.readFromJson(UniLib::convertStringToJson(UniLib::readFileAsString("LoginServer.json")));
		mConnectionNumber = DRINetwork::Instance()->connect(cfg);
		UniLib::EngineLog.writeToLog("connectionNumber get: %d", mConnectionNumber);
		return DR_OK;
        */
	}


	DRReturn LoginTest::test()
	{
		Uint32 startTicks = SDL_GetTicks();
        DRNetServerConfig cfg;
        std::string cfgString = UniLib::readFileAsString("LoginServer.json");
        if(cfgString.size() <= 1) LOG_ERROR("cannot open LoginServer.json", DR_ERROR);
        cfg.readFromJson(UniLib::convertStringToJson(cfgString)["login"]);
		UniLib::server::SektorConnectionManager* sectorConnectionManager = UniLib::server::SektorConnectionManager::getInstance();
        sectorConnectionManager->login("dariofrodo", "h7JD83l29DK", &cfg);
		while(true){//SDL_GetTicks() - startTicks < 10000){
			SDL_Delay(16);
			sectorConnectionManager->condSignal();
		}
        return DR_ERROR;
        /*
		
		//Json::Value getKeyRequest(Json::objectValue);
		Json::FastWriter writer;
		Json::Reader reader;
		Json::Value json;
		
		DRNetRequest keyRequest;
		keyRequest.url = "/spacecraft/serverKeys/get";

		//getKeyRequest["url"] = "/spacecraft/serverKeys/get";
		//getKeyRequest["userAgent"] = UniLib::g_RSAModule->getClientPublicKey();
		
		DRINetwork::Instance()->send(keyRequest, mConnectionNumber);

//		DRINetwork::Instance()->login("dariofrodo", "ssss");

		
		//printf("publickey: %s, privateKey: %s\n", pubKey.data(), privateKey.data());
		unsigned int state = 0;
        
		while(SDL_GetTicks() - startTicks < 10000) 
		{
			std::string recv;
			if(DRINetwork::Instance()->recv(recv, mConnectionNumber) == NET_COMPLETE) {
				// get public key from server
				if(!state) {
					//UniLib::EngineLog.writeToLog("connection recv: %s", recv.data());
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
					Json::Value contentJson(Json::objectValue);
					contentJson["username"] = "admin";
					contentJson["password"] = encrypted;
					//contentJson["publicKey"] = UniLib::g_RSAModule->getClientPrivateKey();
					contentJson["publicKey"] = UniLib::g_RSAModule->getClientPublicKey(UniLib::lib::Crypto::HEX);
					//UniLib::EngineLog.writeToLog(std::string("public key send to server: ") + UniLib::g_RSAModule->getClientPublicKey());
					//UniLib::EngineLog.writeToLog(std::string("public key send to server hex encoded: ") + UniLib::g_RSAModule->getClientPrivateKey());
					loginRequest["url"] = "/spacecraft/playersRaw/login";
					//loginRequest["url"] = "/spacecraft/players/login";
					//loginRequest["url"] = "/spacecraftLoginTest.php";
					loginRequest["method"] = "POST";
					loginRequest["contentType"] = "application/x-www-form-urlencoded";
					loginRequest["userAgent"] = "UniLib::Test";
					//loginRequest["contentType"] = "application/jsonrequest";
					//loginRequest["content"] = encrypted;
					//loginRequest["content"] = "{\"now\":\"entchen\",\"username\":\"einhornimmond\"}";
					loginRequest["content"] = std::string("json=") + writer.write(contentJson);//{\"username\":\"admin\",\"password\":\"") + encrypted + std::string("\"}");
					//loginRequest["content"] = std::string("json={\"username\":\"admin\",\"password\":\"") + std::string("h7JD83l29DK") + std::string("\"}");
					DRINetwork::Instance()->send(writer.write(loginRequest), mConnectionNumber);
					state = 1;
					//login with crypted password
				} else if(state == 1) {
					state = 2;
					//UniLib::EngineLog.writeToLog(std::string("<u>recv login answear: </u>") + recv);
					reader.parse(recv, json);
					std::string state = json.get("status", "not found").asString();
					if(state != std::string("succeed")) {
						UniLib::EngineLog.writeToLog("returned state: %s", state.data());
						UniLib::EngineLog.writeToLog(std::string("error message: ") + json.get("message", "not found").asString());
						LOG_ERROR("Error by login", DR_ERROR);
					}
					std::string requestToken = UniLib::g_RSAModule->crypt(json.get("requestToken", "").asString(), UniLib::lib::Crypto::UNCRYPT_WITH_CLIENT_PRIVATE);
					//UniLib::EngineLog.writeToLog(std::string("encryptet request token: ") + requestToken);
					std::string cryptetRequestToken = UniLib::g_RSAModule->crypt(requestToken, UniLib::lib::Crypto::CRYPT_WITH_SERVER_PUBLIC);
					std::string signature = json.get("signature", "").asString();
					if(!UniLib::g_RSAModule->checkSign(requestToken, signature)) {
						LOG_WARNING("signature isn't valid");
					} else {
						LOG_INFO("signature from request token is valid");
					}
					//UniLib::EngineLog.writeToLog(std::string("Signature: ") + signature);
					Json::Value request(Json::objectValue);
					Json::Value content(Json::objectValue);
					content["requestToken"] = cryptetRequestToken;
					content["request"] = "empty";

					request["url"] = "/spacecraft/requestHandler";
					request["method"] = "POST";
					request["contentType"] = "application/x-www-form-urlencoded";
					request["userAgent"] = "UniLib::Test";
					request["content"] = std::string("json=") + writer.write(content);
					
					DRINetwork::Instance()->send(writer.write(request), mConnectionNumber);
				} else if(state == 2) {
					state = 3;
					UniLib::EngineLog.writeToLog(std::string("recv request answear:<br>") + recv);
					reader.parse(recv, json);
					
				}
			}
			SDL_Delay(100);
		}
		DRINetwork::Instance()->disconnect(mConnectionNumber);

		return DR_ERROR;
        */
	}
}