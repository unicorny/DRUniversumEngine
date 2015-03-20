#include "server/ConnectionToServer.h"
#include "server/SektorConnectionManager.h"
#include "lib/Crypto.h"
#include "lib/DRINetwork.h"
#include "model/SektorID.h"

namespace UniLib {
    namespace server {

        ConnectionToServer::ConnectionToServer(DRNetServerConfig* config)
            : mRSAModule(NULL), mConnectionNumber(-1)
        {
            DRINetwork* network = DRINetwork::getSingletonPtr();
            // create crypto module and generte new keys
            mRSAModule = network->createCrypto();
            if(mRSAModule->generateClientKeys()) LOG_ERROR_VOID("Error by generating client keys");
            // create connection
            mConnectionNumber = network->connect(*config);
            if(mConnectionNumber <= 0) {
                LOG_ERROR_VOID("Error by establish connection to server");
            }
        }

        ConnectionToServer::~ConnectionToServer()
        {
            DRINetwork* network = DRINetwork::getSingletonPtr();
            if(mRSAModule) {
                network->freeCrypto(mRSAModule);
            }
            if(mConnectionNumber >= 0) {
                network->disconnect(mConnectionNumber);
            }
        }

        void ConnectionToServer::sendRequest(DRNetRequest* request, model::SektorID* sektorID, CallbackCommand* callback/* = NULL*/)
        {
            if(mConnectionNumber >= 0) {
                mRequestCommands.push(RequestCommand(request, callback));
                DRINetwork::Instance()->send(*request, mConnectionNumber);
            } else {
                LOG_ERROR_VOID("Error by sending request, no connection found");
            }            
        }
        DRReturn ConnectionToServer::update()
        {
            if(mRequestCommands.size() > 0) {
                std::string dataBuffer;
                DRNet_Status status = DRINetwork::Instance()->recv(dataBuffer, mConnectionNumber);
                if(status != NET_NOT_READY) return DR_OK;
                if(mRequestCommands.top().command) {
                    mRequestCommands.top().command->execute(status, dataBuffer);
                }
                //mRequestCommands
                SektorConnectionManager::getInstance()->freeNetRequest(mRequestCommands.top().request);
                mRequestCommands.pop();
            }
            return DR_OK;
        }
    }
}