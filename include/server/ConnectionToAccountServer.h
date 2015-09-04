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
 * \date 19.03.15
 * 
 * \desc Class for Connection to Account Server
 */

#ifndef __UNIVERSUM_LIB_SERVER_CONNECTION_TO_ACCOUNT_SERVER_H
#define __UNIVERSUM_LIB_SERVER_CONNECTION_TO_ACCOUNT_SERVER_H

#include "ConnectionToServer.h"
#include "controller/NetworkTask.h"

namespace UniLib {
	namespace controller {
		//class UNIVERSUM_LIB_API NetworkTask;

	}
    namespace server {
		class LoginNetworkTask;
		/*! 
		 * \author: Dario Rekowski
		 *
		 * \date: 04.09.2015
		 * \brief Connection to account server, login, stats and getting addresses for other server
		 */
        class UNIVERSUM_LIB_API ConnectionToAccountServer: public ConnectionToServer
        {
        public:
            ConnectionToAccountServer(const DRNetServerConfig* serveConfig);
            virtual ~ConnectionToAccountServer();

			// derived from parent
			virtual DRReturn init();
			virtual DRReturn update();

			// login
			void login(const char* username, const char* password, CallbackCommand* command);


            __inline__ bool isLogin() {return mSuccesfullyLoggedIn;}
        protected:
			// member functions, derived from parent
			virtual void additionalFieldsAndCryptRequest(DRNetRequest* netRequest);

            bool mSuccesfullyLoggedIn;
			// getting from server after login, needed for all requests
			LoginNetworkTask* mLogin;

        };
		/*! 
		 * \author: Dario Rekowski
		 *
		 * \date: 04.09.2015
		 * \brief Network Task for logging in
		 */
		class UNIVERSUM_LIB_API LoginNetworkTask : public controller::NetworkTask
		{
		public:
			LoginNetworkTask(ConnectionToAccountServer* parent, const char* username, const char* password);
			virtual ~LoginNetworkTask() {};

			virtual DRReturn run();
			virtual const char* getResourceType() const {return "LoginNetworkTask";};
		protected:
			__inline__ virtual void scheduleTask() {mParent->scheduleNetworkTask(this);};
			DRString mUsername;
			DRString mPassword;
			ConnectionToAccountServer* mParent;
		};
    }
}



#endif //__UNIVERSUM_LIB_SERVER_CONNECTION_TO_ACCOUNT_SERVER_H