/*/*************************************************************************
*                                                                         *
* UniversumLib, collection of classes for generating and go through a     *
* whole universe. It is for my Gameproject Spacecraft					   *
* Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
* Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
*                                                                         *
* This program is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* any later version.													   *
*																		   *
* This program is distributed in the hope that it will be useful,	       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
* GNU General Public License for more details.							   *
*																		   *
* You should have received a copy of the GNU General Public License	   *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                         *
***************************************************************************/

/**
 * \author Dario Rekowski
 * 
 * \date 11.03.15
 * 
 * \desc Singleton Server Connection Manage, managed various Server connections
 */

#ifndef __DR_UNIVERSUM_LIB_SERVER_SEKTOR_CONNECTION_MANAGER_H__
#define __DR_UNIVERSUM_LIB_SERVER_SEKTOR_CONNECTION_MANAGER_H__

#include "lib/Singleton.h"
#include "lib/Thread.h"

#define NET_REQUEST_MEMORY_LIST_OBJECT_COUNT 128

struct DRNetRequest;
struct DRNetServerConfig;

namespace UniLib {
    namespace model {
        class SektorID;
    }
	namespace server {
        class ConnectionToAccountServer;
        class CallbackCommand;

        enum RequestType {
             REQUEST_TYPE_NONE = 0,
             REQUEST_TYPE_LOGIN = 1,
             REQUEST_TYPE_ACCOUNT_DATA = 2,
             REQUEST_TYPE_BLOCK_DATA = 3,
             REQUEST_TYPE_LIVE_DATA = 4
        };

		class UNIVERSUM_LIB_API SektorConnectionManager: public lib::Singleton, public lib::Thread
		{
		public:
			static SektorConnectionManager* const getInstance();

            // login
            void login(const char* username, const char* password, DRNetServerConfig* accountServerConfig, CallbackCommand* callback = NULL);
            __inline__ bool isLogin() {return mLoginSuccessfully;}

            // default request
            //DRReturn sendRequest(DRNetRequest* request, RequestType type, model::SektorID* sektorID, CallbackCommand* callback = NULL);

            // memory management
            DRNetRequest* getFreeNetRequest();
            void freeNetRequest(DRNetRequest* request);

        protected:
            virtual int ThreadFunction();

		private:
			SektorConnectionManager();
			virtual ~SektorConnectionManager();

			// member variables
            // Connections
            ConnectionToAccountServer* mAccountServer;

            // state variables
            bool    mLoginSuccessfully;
			bool    mInitalized;
            
            // memory management
            DRMemoryList<DRNetRequest> mNetRequestsMemoryList;

			// request waiting to getting startet
			//struct 
		};
	};
};

#endif //__DR_UNIVERSUM_LIB_SERVER_SEKTOR_CONNECTION_MANAGER_H__