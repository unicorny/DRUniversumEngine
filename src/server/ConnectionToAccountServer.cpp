#include "server/ConnectionToAccountServer.h"
#include "server/SektorConnectionManager.h"
#include "controller/NetworkTask.h"
#include "lib/Crypto.h"
#include "lib/CommandEventManager.h"

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
			if(!isRequestSend()) {
				// build datas for request and send it out
				Uint32 start_ticks = SDL_GetTicks();
				Json::FastWriter writer;
				Json::Value contentJson(Json::objectValue);
				lib::Crypto* rsa = mParent->getRSAModule();
				contentJson["username"] = mUsername;
				std::string encrypted = rsa->crypt(mPassword, lib::Crypto::CRYPT_WITH_SERVER_PUBLIC);
				contentJson["password"] = encrypted;
				contentJson["publicKey"] = rsa->getClientPublicKey(lib::Crypto::HEX);
				lock();
				mRequest.content = contentJson;
				unlock();
				EngineLog.writeToLog("[LoginNetworkTask::run] %d ms", (SDL_GetTicks()-start_ticks));
				return NetworkTask::run();
			} else if(mResult.size() > 0 && mRequestKey.size() == 0) {
				// process recv data
				Uint32 start_ticks = SDL_GetTicks();
				Json::Value json = convertStringToJson(mResult);
				EngineLog.writeToLog("[LoginNetworkTask::run] convert string to json %d ms", (SDL_GetTicks()-start_ticks));
				start_ticks = SDL_GetTicks();
				if(json.get("state", "failed").asString() == std::string("succeed")) {
					lib::Crypto* rsa = mParent->getRSAModule();
					lock();
					mRequestKey = rsa->crypt(json.get("requestToken", "").asString(), UniLib::lib::Crypto::UNCRYPT_WITH_CLIENT_PRIVATE);
					EngineLog.writeToLog("[LoginNetworkTask::run] uncrypt request token %d ms", (SDL_GetTicks()-start_ticks));
					start_ticks = SDL_GetTicks();
					std::string cryptetRequestToken = rsa->crypt(mRequestKey, UniLib::lib::Crypto::CRYPT_WITH_SERVER_PUBLIC);
					EngineLog.writeToLog("[LoginNetworkTask::run] crypt request token %d ms", (SDL_GetTicks()-start_ticks));
					start_ticks = SDL_GetTicks();
					std::string signature = json.get("signature", "").asString();
					if(!rsa->checkSign(mRequestKey, signature)) {
						LOG_WARNING("signature isn't valid");
						mRequestKey.clear();
					} else {
						LOG_INFO("signature from request token is valid");
						mResult = mRequestKey;
					}
					unlock();
					EngineLog.writeToLog("[LoginNetworkTask::execute] check sign %d ms", (SDL_GetTicks()-start_ticks));
					start_ticks = SDL_GetTicks();

				}
				if(json["state"].asString() == std::string("failed")) {
					EngineLog.writeToLog("request failed with message: %s", json["message"].asCString());
					LOG_WARNING("login failed");
				} else {

				}
				EngineLog.writeToLog("[LoginNetworkTask::execute] %d ms", (SDL_GetTicks()-start_ticks));
				if(mFinishCommand) {
					mFinishCommand->taskFinished(this);
					DR_SAVE_DELETE(mFinishCommand);
				}
			}
			return DR_ERROR;
		}

		bool LoginNetworkTask::isTaskFinished()
		{
			bool finished = false;
			lock();
			finished = mRequestKey.size() > 0;
			unlock();
			return finished;
		}

	


		// ****************************************************************************************************************************************
		// Connection To Account Server
		// ****************************************************************************************************************************************
        ConnectionToAccountServer::ConnectionToAccountServer(const DRNetServerConfig* serveConfig, lib::CommandEventManager* eventManager)
            : ConnectionToServer(serveConfig, eventManager), mSuccesfullyLoggedIn(false)
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
		void ConnectionToAccountServer::login(const char* username, const char* password)
		{
			mLogin = new LoginNetworkTask(this, username, password);
			mLogin->setFinishCommand(new lib::CallEventManagerCommand("login", mLogin, mEventManager));
			scheduleNetworkTask(mLogin);
		}
		DRReturn ConnectionToAccountServer::update()
		{

			return ConnectionToServer::update();
		}
		// ********************************************************************
		// protected Member functions 
		// ********************************************************************

	}
}