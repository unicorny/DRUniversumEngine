#include "server/ConnectionToServer.h"
#include "server/SektorConnectionManager.h"
#include "lib/Crypto.h"
#include "lib/DRINetwork.h"
#include "model/SektorID.h"

namespace UniLib {
    namespace server {

        ConnectionToServer::ConnectionToServer(const DRNetServerConfig* config)
            : mRSAModule(NULL), mConnectionNumber(-1), mWorkMutex(SDL_CreateMutex()), mPendingWorkMutex(SDL_CreateMutex()), mInitalized(false)
        {       
            DRINetwork* network = DRINetwork::getSingletonPtr();
            // create crypto module
            mRSAModule = network->createCrypto();

			// copy server config
            mServerConfig.port = config->port;
			mServerConfig.url = config->url;
			mServerConfig.serverType = config->serverType;                  
        }

        DRReturn ConnectionToServer::init()
        {
			if(mInitalized) LOG_ERROR("connection already initalized", DR_ERROR);
			DRINetwork* network = DRINetwork::getSingletonPtr();
			// generate new keys for crypto
            if(mRSAModule->generateClientKeys()) 
                LOG_ERROR("Error by generating client keys", DR_ERROR);

			// create connection
			mConnectionNumber = network->connect(mServerConfig);
			if(mConnectionNumber <= 0) {
				LOG_ERROR("Error by establish connection to server", DR_ERROR);
			}
            mInitalized = true;
            return DR_OK;
        }

        ConnectionToServer::~ConnectionToServer()
        {
			SDL_LockMutex(mPendingWorkMutex);
            SDL_LockMutex(mWorkMutex);
            DRINetwork* network = DRINetwork::getSingletonPtr();
            if(mRSAModule) {
                network->freeCrypto(mRSAModule);
            }
            if(mConnectionNumber >= 0) {
                network->disconnect(mConnectionNumber);
            }
            cleanUpRequestCommandQueue(mRequestCommands);
			cleanUpRequestCommandQueue(mPendingRequestCommands);
            
            mInitalized = false;
            SDL_UnlockMutex(mWorkMutex);            
            SDL_DestroyMutex(mWorkMutex);    
			SDL_UnlockMutex(mPendingWorkMutex);
			SDL_DestroyMutex(mPendingWorkMutex);
        }

        void ConnectionToServer::sendRequest(DRNetRequest* request, model::SektorID* sektorID, CallbackCommand* callback/* = NULL*/)
        {
            if(!mInitalized) LOG_ERROR_VOID("not initalized yet");
            SDL_LockMutex(mPendingWorkMutex);
			mPendingRequestCommands.push(RequestCommand(request, callback));                   
            SDL_UnlockMutex(mPendingWorkMutex);
        }
		void ConnectionToServer::sendRequestDirect(DRNetRequest* request, CallbackCommand* callback/* = NULL*/)
		{
			SDL_LockMutex(mWorkMutex);
			DRINetwork::Instance()->send(*request, mConnectionNumber);
			mRequestCommands.push(RequestCommand(request, callback));
			SDL_UnlockMutex(mWorkMutex);
		}
        DRReturn ConnectionToServer::update()
        {
            if(!mInitalized) {
				DRReturn result = init();
				if(result) LOG_ERROR("error by init Connection To Server", result);
			}
            SDL_LockMutex(mPendingWorkMutex);
            // if we have pending request
			if(mPendingRequestCommands.size() && mConnectionNumber >= 0 && mRSAModule->isServerPublicKeyExist() ) {
				DRNetRequest* request = mPendingRequestCommands.front().request;
				//request->content
				additionalFieldsAndCryptRequest(request);
				sendRequestDirect(request, mPendingRequestCommands.front().command);
				mPendingRequestCommands.pop();				
			} 
			SDL_UnlockMutex(mPendingWorkMutex);
			// if there were data to receive
			SDL_LockMutex(mWorkMutex);
            while(mRequestCommands.size() > 0) {
                std::string dataBuffer;
                DRNet_Status status = DRINetwork::Instance()->recv(dataBuffer, mConnectionNumber);
                if(status != NET_NOT_READY) return DR_OK;
                if(mRequestCommands.front().command) {
                    SDL_UnlockMutex(mWorkMutex);
                    mRequestCommands.front().command->execute(status, dataBuffer);
                    SDL_LockMutex(mWorkMutex);
                }
                //mRequestCommands
                SektorConnectionManager::getInstance()->freeNetRequest(mRequestCommands.front().request);
                mRequestCommands.pop();
            }
            SDL_UnlockMutex(mWorkMutex);
            return DR_OK;
        }

		// **************************************************************************************************************
		// protected member area
		// **************************************************************************************************************
		void ConnectionToServer::cleanUpRequestCommandQueue(std::queue<RequestCommand>& requestCommandQueue)
		{
			SektorConnectionManager* parent = SektorConnectionManager::getInstance();
			while(requestCommandQueue.size()) {
				parent->freeNetRequest(requestCommandQueue.front().request);
				if(requestCommandQueue.front().command) {
					std::string stringBuffer("ende");
					SDL_UnlockMutex(mWorkMutex);
					requestCommandQueue.front().command->execute(NET_CONNECTION_CLOSED, stringBuffer);
					SDL_LockMutex(mWorkMutex);
				}
				requestCommandQueue.pop();
			}
		}
		// ***************************************************************************************************************
		// commands
		// ***************************************************************************************************************
		void ConnectionToServer::GetPublicKeyCommand::execute(DRNet_Status status, std::string& data)
		{
			if(status == NET_COMPLETE) {
				if(mParent->mRSAModule->setServerPublicKey(data)) {
					delete this;
					LOG_ERROR_VOID("setting server public key failed with data: %s", data.data());
				}
			}
			delete this;
			LOG_ERROR_VOID("setting server public key failed with status: %d", status); 
		}
    }
}