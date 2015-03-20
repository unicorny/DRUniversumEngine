#include "server/ConnectionToServer.h"
#include "server/SektorConnectionManager.h"
#include "lib/Crypto.h"
#include "lib/DRINetwork.h"
#include "model/SektorID.h"

namespace UniLib {
    namespace server {

        ConnectionToServer::ConnectionToServer(DRNetServerConfig* config)
            : mRSAModule(NULL), mConnectionNumber(-1), mWorkMutex(SDL_CreateMutex()), mInitalized(false)
        {       
            DRINetwork* network = DRINetwork::getSingletonPtr();
            // create crypto module and generte new keys
            mRSAModule = network->createCrypto();
            
            // create connection
            mConnectionNumber = network->connect(*config);
            if(mConnectionNumber <= 0) {
                LOG_ERROR_VOID("Error by establish connection to server");
            }      
        }

        DRReturn ConnectionToServer::init()
        {
            if(mRSAModule->generateClientKeys()) 
                LOG_ERROR("Error by generating client keys", DR_ERROR);
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
            SektorConnectionManager* parent = SektorConnectionManager::getInstance();
            while(mRequestCommands.size()) {
                parent->freeNetRequest(mRequestCommands.top().request);
                if(mRequestCommands.top().command) {
                    std::string stringBuffer("ende");
                    SDL_UnlockMutex(mWorkMutex);
                    mRequestCommands.top().command->execute(NET_CONNECTION_CLOSED, stringBuffer);
                    SDL_LockMutex(mWorkMutex);
                }
                mRequestCommands.pop();
            }
            mInitalized = false;
            SDL_UnlockMutex(mWorkMutex);            
            SDL_DestroyMutex(mWorkMutex);            
        }

        void ConnectionToServer::sendRequest(DRNetRequest* request, model::SektorID* sektorID, CallbackCommand* callback/* = NULL*/)
        {
            if(!mInitalized) LOG_ERROR_VOID("not initalized yet");
            SDL_LockMutex(mWorkMutex);
            if(mConnectionNumber >= 0) {
                mRequestCommands.push(RequestCommand(request, callback));
                DRINetwork::Instance()->send(*request, mConnectionNumber);
            } else {
                LOG_ERROR_VOID("Error by sending request, no connection found");
            }       
            SDL_UnlockMutex(mWorkMutex);
        }
        DRReturn ConnectionToServer::update()
        {
            if(!mInitalized) return DR_OK;
            SDL_LockMutex(mWorkMutex);
            // if we have pending request
            while(mRequestCommands.size() > 0) {
                std::string dataBuffer;
                DRNet_Status status = DRINetwork::Instance()->recv(dataBuffer, mConnectionNumber);
                if(status != NET_NOT_READY) return DR_OK;
                if(mRequestCommands.top().command) {
                    SDL_UnlockMutex(mWorkMutex);
                    mRequestCommands.top().command->execute(status, dataBuffer);
                    SDL_LockMutex(mWorkMutex);
                }
                //mRequestCommands
                SektorConnectionManager::getInstance()->freeNetRequest(mRequestCommands.top().request);
                mRequestCommands.pop();
            }
            SDL_UnlockMutex(mWorkMutex);
            return DR_OK;
        }
    }
}