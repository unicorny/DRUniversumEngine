#include "server/SektorConnectionManager.h"
#include "server/ConnectionToAccountServer.h"

namespace UniLib {
	namespace server {
		SektorConnectionManager::SektorConnectionManager()
            : mAccountServer(NULL), mLoginSuccessfully(false), mNetRequestsMemoryList(NET_REQUEST_MEMORY_LIST_OBJECT_COUNT)
		{
            mAccountServer = new ConnectionToAccountServer;
		}

		SektorConnectionManager::~SektorConnectionManager() 
		{
            DR_SAVE_DELETE(mAccountServer); 
		}

		const SektorConnectionManager* SektorConnectionManager::getInstance()
		{
			if(!mpInstanz) {
				mpInstanz = new SektorConnectionManager;
			}

			return (SektorConnectionManager*)mpInstanz;
		}

         int SektorConnectionManager::ThreadFunction()
         {
             mAccountServer->update();
             // return good
             return 0;
         }
         void SektorConnectionManager::login(const char* username, const char* password, CallbackCommand* callback/* = NULL*/)
         {
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
             return mNetRequestsMemoryList.NewInstance();
         }
         void SektorConnectionManager::freeNetRequest(DRNetRequest* request)
         {
             mNetRequestsMemoryList.FreeInstance(request);
         }

	}
}

