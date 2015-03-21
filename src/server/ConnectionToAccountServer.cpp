#include "server/ConnectionToAccountServer.h"
#include "server/SektorConnectionManager.h"

namespace UniLib {
    namespace server {

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

			DRNetRequest* netRequest = SektorConnectionManager::getInstance()->getFreeNetRequest();
			if(netRequest) {
				// send request to getting public key
				netRequest->url = "/spacecraft/serverKeys/get";
				netRequest->method = NET_REQUEST_METHOD_GET;
				sendRequestDirect(netRequest, new GetPublicKeyCommand(this));
			}
			
			return DR_OK;
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