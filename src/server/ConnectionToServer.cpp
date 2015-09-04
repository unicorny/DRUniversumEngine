#include "server/ConnectionToServer.h"
#include "server/SektorConnectionManager.h"
#include "lib/Crypto.h"
#include "lib/DRINetwork.h"
#include "model/SektorID.h"

namespace UniLib {
    namespace server {

        ConnectionToServer::ConnectionToServer(const DRNetServerConfig* config)
            : mRSAModule(NULL), mConnectionNumber(-1), mWorkMutex(SDL_CreateMutex()), mInitalized(false)
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
			DRNetRequest request;
			request.url = "/spacecraft/serverKeys/get";
			request.method = NET_REQUEST_METHOD_GET;
			mPublicKeyRequestTask = new GetPublicKeyNetworkTask(this, request, mConnectionNumber);
			mPublicKeyRequestTask->run();
            mInitalized = true;
            return DR_OK;
        }
        ConnectionToServer::~ConnectionToServer()
        {

            SDL_LockMutex(mWorkMutex);
            DRINetwork* network = DRINetwork::getSingletonPtr();
            if(mRSAModule) {
                network->freeCrypto(mRSAModule);
            }
            if(mConnectionNumber >= 0) {
                network->disconnect(mConnectionNumber);
            }
            
            mInitalized = false;
            SDL_UnlockMutex(mWorkMutex);            
            SDL_DestroyMutex(mWorkMutex);    
        }

		void ConnectionToServer::scheduleNetworkTask(controller::NetworkTask* networkTask)
		{
			SDL_LockMutex(mWorkMutex);
			mNetworkTasks.push_back(networkTask);
			SDL_UnlockMutex(mWorkMutex);
		}

        DRReturn ConnectionToServer::update()
        {
            if(!mInitalized) {
				DRReturn result = init();
				if(result) LOG_ERROR("error by init Connection To Server", result);
			}
			SDL_LockMutex(mWorkMutex);
			for(std::list<controller::NetworkTask*>::iterator it = mNetworkTasks.begin(); it != mNetworkTasks.end(); it++) {
				controller::NetworkTask* task = *it;
				if(task->isTaskFinished()) {
					it = mNetworkTasks.erase(it);
				} else if(task->isAllParentsReady()) {
					task->run();
				}
			}
            SDL_UnlockMutex(mWorkMutex);
            return DR_OK;
        }

		// **************************************************************************************************************
		// protected member area
		// **************************************************************************************************************
		
		// ***************************************************************************************************************
		// commands
		// ***************************************************************************************************************
		void GetPublicKeyNetworkTask::execute(DRNet_Status status, std::string& data)
		{
			lock();
			if(status == NET_COMPLETE) {
				if(mParent->getRSAModule()->setServerPublicKey(data)) {
                    DRLog.writeToLog("setting server public key failed with data: %s", data.data());
					LOG_WARNING("failed with setting server key");
				} else {
					mResult = data;
				}
			} else {
				DRLog.writeToLog("setting server public key failed with status: %d", status);
				LOG_WARNING("failed with setting server key"); 
			}
			unlock();
		}
    }
}