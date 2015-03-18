#include "server/ConnectionToServer.h"
#include "lib/Crypto.h"
#include "lib/DRINetwork.h"

namespace UniLib {
    namespace server {

        ConnectionToServer::ConnectionToServer()
            : mRSAModule(NULL)
        {
            mRSAModule = DRINetwork::Instance()->createCrypto();
        }

        ConnectionToServer::~ConnectionToServer()
        {
            if(mRSAModule) {
                DRINetwork::Instance()->freeCrypto(mRSAModule);
            }
        }
    }
}