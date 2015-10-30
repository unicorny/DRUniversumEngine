#include "server/SektorConnectionManager.h"
#include "server/ConnectionToAccountServer.h"
#include "lib/CommandEventManager.h"

namespace UniLib {
	namespace server {

		SektorConnectionManager* SektorConnectionManager::mpInstanz = NULL;

		SektorConnectionManager::SektorConnectionManager()
            : Thread("UniSecCon"), mAccountServer(NULL), mLoginSuccessfully(false), mInitalized(false), 
			mNetRequestsMemoryList(NET_REQUEST_MEMORY_LIST_OBJECT_COUNT), mEventManager(NULL)
		{
            
		}

		SektorConnectionManager::~SektorConnectionManager() 
		{
            DR_SAVE_DELETE(mAccountServer); 
			if(mEventManager) mEventManager->exit();
			DR_SAVE_DELETE(mEventManager);
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
		 DRReturn SektorConnectionManager::init()
		 {
			 mEventManager = new lib::CommandEventManager;
			 mEventManager->init();
			 return DR_OK;
		 }
         void SektorConnectionManager::login(const char* username, const char* password, DRNetServerConfig* accountServerConfig)
         {
			 if(mAccountServer && mAccountServer->isLogin()) return;
			 if(!mAccountServer) {
				mAccountServer = new ConnectionToAccountServer(accountServerConfig, mEventManager);
				mAccountServer->init();
				mAccountServer->login(username, password);
			 }
			 
             //DRNetRequest* request = getFreeNetRequest();
             //request->method         
         }
       /*  DRReturn SektorConnectionManager::sendRequest(DRNetRequest* request, RequestType type, model::SektorID* sektorID, CallbackCommand* callback)
         {
             lock();
             switch(type) {
             case REQUEST_TYPE_LOGIN: mAccountServer->sendRequest(request, sektorID, callback);
             }
             unlock();
             return DR_OK;
         }
	   */
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

