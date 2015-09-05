#include "server/ConnectionToAccountServer.h"
#include "server/SektorConnectionManager.h"
#include "controller/NetworkTask.h"
#include "lib/Crypto.h"

namespace UniLib {
    namespace server {

		// ****************************************************************************************************************************************
		// Login Network Task
		// ****************************************************************************************************************************************
		LoginNetworkTask::LoginNetworkTask(ConnectionToAccountServer* parent, const char* username, const char* password)
			: NetworkTask(1, parent->getConnectionNumber()), mUsername(username), mPassword(password), mParent(parent)
		{
			setParentTaskPtrInArray(new DRResourcePtrHolder(parent->getPublickKeyTask()), 0);
			mRequest.url = "/spacecraft/playersRaw/login";
			mRequest.method = NET_REQUEST_METHOD_POST;
			mRequest.contentType = NET_REQUEST_CONTENT_TYPE_FORM_URLENCODED;
			mRequest.userAgent = "UniLib::Test";
		}

		DRReturn LoginNetworkTask::run()
		{
			if(isRequestSend()) return DR_OK;
			Json::FastWriter writer;
			Json::Value contentJson(Json::objectValue);
			lib::Crypto* rsa = mParent->getRSAModule();
			contentJson["username"] = mUsername;
			std::string encrypted = rsa->crypt(mPassword, lib::Crypto::CRYPT_WITH_SERVER_PUBLIC);
			contentJson["password"] = encrypted;
			contentJson["publicKey"] = rsa->getClientPublicKey(lib::Crypto::HEX);
			mRequest.content = contentJson;
			return NetworkTask::run();
		}

		void LoginNetworkTask::execute(DRNet_Status status, std::string& data)
		{
			Json::Value json = convertStringToJson(data);
			if(json["state"].asString() == std::string("failed")) {
				EngineLog.writeToLog("request failed with message: %s", json["message"].asCString());
				LOG_WARNING("login failed");
			}
		}


		// ****************************************************************************************************************************************
		// Connection To Account Server
		// ****************************************************************************************************************************************
        ConnectionToAccountServer::ConnectionToAccountServer(const DRNetServerConfig* serveConfig)
            : ConnectionToServer(serveConfig), mSuccesfullyLoggedIn(false)
        {
        }

        ConnectionToAccountServer::~ConnectionToAccountServer()
        {
        }

		DRReturn ConnectionToAccountServer::init()
		{
			DRReturn result = ConnectionToServer::init();
			if(result) LOG_ERROR("Error by initing parent class", DR_ERROR);

			//DRNetRequest* netRequest = SektorConnectionManager::getInstance()->getFreeNetRequest();
			return DR_OK;
		}
		void ConnectionToAccountServer::login(const char* username, const char* password, CallbackCommand* command)
		{
			mLogin = new LoginNetworkTask(this, username, password);
			scheduleNetworkTask(mLogin);
		}
		DRReturn ConnectionToAccountServer::update()
		{

			return ConnectionToServer::update();
		}
		// ********************************************************************
		// protected Member functions 
		// ********************************************************************
	    void  ConnectionToAccountServer::additionalFieldsAndCryptRequest(DRNetRequest* netRequest)
		{

		}
    }
}