#include "server/SektorConnectionManager.h"
#include "server/ConnectionToAccountServer.h"

namespace UniLib {
	namespace server {
		SektorConnectionManager::SektorConnectionManager()
            : mAccountServer(NULL), mLoginSuccessfully(false), mInitalized(false), mNetRequestsMemoryList(NET_REQUEST_MEMORY_LIST_OBJECT_COUNT)
		{
            
		}

		SektorConnectionManager::~SektorConnectionManager() 
		{
            DR_SAVE_DELETE(mAccountServer); 
		}

		SektorConnectionManager* const SektorConnectionManager::getInstance()
		{
			if(!mpInstanz) {
				mpInstanz = new SektorConnectionManager;
			}

			return (SektorConnectionManager*)mpInstanz;
		}

         int SektorConnectionManager::ThreadFunction()
         {			 			  
			 if(mAccountServer) {
				mAccountServer->update();
			 }
             // return good
             return 0;
         }
         void SektorConnectionManager::login(const char* username, const char* password, DRNetServerConfig* accountServerConfig, CallbackCommand* callback/* = NULL*/)
         {
			 if(mAccountServer && mAccountServer->isLogin()) return;
			 if(!mAccountServer) {
				mAccountServer = new ConnectionToAccountServer(accountServerConfig);
			 }
			 
			 
             DRNetRequest* request = getFreeNetRequest();
             //request->method         
         }
         DRReturn SektorConnectionManager::sendRequest(DRNetRequest* request, RequestType type, model::SektorID* sektorID, CallbackCommand* callback/* = NULL*/)
         {
             lock();
             switch(type) {
             case REQUEST_TYPE_LOGIN: mAccountServer->sendRequest(request, sektorID, callback);
             }
             unlock();
             return DR_OK;
         }

         // memory management
         DRNetRequest* SektorConnectionManager::getFreeNetRequest()
         {
			 DRNetRequest* request = mNetRequestsMemoryList.NewInstance();
			 if(!request) LOG_ERROR("no space left for a new net request", NULL);
			 return request;
         }
         void SektorConnectionManager::freeNetRequest(DRNetRequest* request)
         {
             mNetRequestsMemoryList.FreeInstance(request);
         }

	}
}

