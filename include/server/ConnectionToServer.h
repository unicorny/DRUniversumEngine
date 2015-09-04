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
 * \date 18.03.15
 * 
 * \desc Base Class for Server Connection Classes
 */

#ifndef __UNIVERSUM_LIB_SERVER_CONNECTION_TO_SERVER__H
#define __UNIVERSUM_LIB_SERVER_CONNECTION_TO_SERVER__H

#include "Callbacks.h"
#include "UniversumLib.h"
#include "controller/NetworkTask.h"

struct DRNetServerConfig;

namespace UniLib {
    namespace lib {
        class Crypto;
    }
    namespace model {
        class SektorID;
    }

    namespace server {

		class UNIVERSUM_LIB_API ConnectionToServer;
		class GetPublicKeyNetworkTask : public controller::NetworkTask
		{
		public:
			GetPublicKeyNetworkTask(ConnectionToServer* parent, int connectionNumber);
			virtual ~GetPublicKeyNetworkTask() {}

			virtual void execute(DRNet_Status status, std::string& data);
			virtual const char* getResourceType() const {return "GetPublicKeyNetworkTask";};
		protected:
			ConnectionToServer* mParent;
		};

		/*!
		 * \author Dario Rekowski
		 *
		 * \date 09.04.15
		 *
		 * \brief connection to server base class
		 * 
		 */
        class UNIVERSUM_LIB_API ConnectionToServer
        {
        public:
            ConnectionToServer(const DRNetServerConfig* config);
            virtual ~ConnectionToServer();

            virtual DRReturn init();


			virtual void scheduleNetworkTask(controller::NetworkTask* networkTask);
            //! \brief called every time a new packet came from network hardware
			//! check if request in queue, if then call additionalFieldsAndCryptRequest and sendRequestDirect
			//! check if request answers existing, if then call callbackCommand and remove request and command afterwards
            virtual DRReturn update();

			// inline check state
            __inline__ bool isConnectionValid()  {return mConnectionNumber >= 0;}
            __inline__ bool isInitalized() {return mInitalized;}

			// getter and setter for network tasks
			__inline__ u16 getConnectionNumber() {return mConnectionNumber;}
			__inline__ GetPublicKeyNetworkTask* getPublickKeyTask() { return mPublicKeyRequestTask;}
			__inline__ lib::Crypto* getRSAModule() {return mRSAModule;}
			__inline__ DRNetServerConfig getServerConfig() {return mServerConfig;}
			
        protected:
			// member structures
			// request command structure
			struct RequestCommand {
				RequestCommand(DRNetRequest* request, CallbackCommand* callback)
					: request(request), command(callback) {}
				DRNetRequest* request;
				CallbackCommand* command;
			};

			// member function
			//! \brief remove all request command from queue and call them with NET_CONNECTION_CLOSED
			//! called on exit

			virtual void additionalFieldsAndCryptRequest(DRNetRequest* netRequest) = 0;
			void sendRequestDirect(DRNetRequest* request, CallbackCommand* callback = NULL);

            // crypto modul
			lib::Crypto* mRSAModule;
			// connection number
			u16          mConnectionNumber;
			// connection config
			DRNetServerConfig mServerConfig;
			// mutex
			SDL_mutex*   mWorkMutex;

			// initalized
			bool         mInitalized;

			std::list<controller::NetworkTask*> mNetworkTasks;
			// public key
			GetPublicKeyNetworkTask* mPublicKeyRequestTask;
        };
    }
}

#endif //__UNIVERSUM_LIB_SERVER_CONNECTION_TO_SERVER__H