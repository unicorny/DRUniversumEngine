#include "server/SektorConnectionManager.h"
#include "server/ConnectionToAccountServer.h"

namespace UniLib {
	namespace server {
		SektorConnectionManager::SektorConnectionManager()
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
	}
}

