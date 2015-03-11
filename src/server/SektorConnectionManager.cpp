#include "server/SektorConnectionManager.h"

namespace UniLib {
	namespace server {
		SektorConnectionManager::SektorConnectionManager()
		{

		}

		SektorConnectionManager::~SektorConnectionManager() 
		{

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

