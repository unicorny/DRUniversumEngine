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

struct DRNetRequest;

namespace UniLib {
    namespace model {
        class SektorID;
    }
	namespace server {
        class ConnectionToServer;
        class ConnectionToAccountServer;
        class CallbackCommand;

        enum ConnectionType {
             CONNECTION_NONE = 0,
             CONNECTION_TO_LIVE_SERVER = 1,
             CONNECTION_TO_WORLD_DATA_SERVER = 2,
             CONNECTION_TO_ACCOUNT_SERVER = 3
        };

		class UNIVERSUM_LIB_API SektorConnectionManager: public lib::Singleton
		{
		public:
			const static SektorConnectionManager* getInstance();

            // login
            void login(const char* username, const char* password, CallbackCommand* callback = NULL);
            __inline__ bool isLogin() {return mAccountServer->isLogin();}

            // default request
            DRReturn sendRequest(DRNetRequest* request, model::SektorID* sektorID, CallbackCommand* callback = NULL);

		private:
			SektorConnectionManager();
			virtual ~SektorConnectionManager();

			// member variables
            // Connections
            ConnectionToAccountServer* mAccountServer;

            // Request answear stacks
            struct RequestCommand {
                DRNetRequest* request;
                CallbackCommand* command;
            };
            std::stack<RequestCommand> mRequestCommands;
		};
	};
};

#endif //__DR_UNIVERSUM_LIB_SERVER_SEKTOR_CONNECTION_MANAGER_H__