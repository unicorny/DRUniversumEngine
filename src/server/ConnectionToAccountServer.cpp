#include "server/ConnectionToAccountServer.h"
#include "server/SektorConnectionManager.h"
#include "controller/NetworkTask.h"

namespace UniLib {
    namespace server {

		// ****************************************************************************************************************************************
		// Login Network Task
		// ****************************************************************************************************************************************
		LoginNetworkTask::LoginNetworkTask(ConnectionToAccountServer* parent, const char* username, const char* password)
			: NetworkTask(1, parent->getConnectionNumber()), mUsername(username), mPassword(password), mParent(parent)
		{
			setParentTaskPtrInArray(new DRResourcePtrHolder(parent->getPublickKeyTask()), 0);
		}

		DRReturn LoginNetworkTask::run()
		{
			return DR_ERROR;
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