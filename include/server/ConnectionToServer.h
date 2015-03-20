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

struct DRNetServerConfig;

namespace UniLib {
    namespace lib {
        class Crypto;
    }
    namespace model {
        class SektorID;
    }
    namespace server {

        class UNIVERSUM_LIB_API ConnectionToServer
        {
        public:
            ConnectionToServer(DRNetServerConfig* config);
            virtual ~ConnectionToServer();

            virtual void sendRequest(DRNetRequest* request, model::SektorID* sektorID, CallbackCommand* callback = NULL);

            // called every time a new packet came from network hardware
            virtual DRReturn update();

            __inline__ bool isConnectionValid()  {return mConnectionNumber >= 0;}
        protected:
            // crypto modul
           lib::Crypto* mRSAModule;
           // connection number
           int          mConnectionNumber;
           // mutex
           SDL_mutex*   mWorkMutex;

           // Request answear stacks
           struct RequestCommand {
               RequestCommand(DRNetRequest* request, CallbackCommand* callback)
                   : request(request), command(callback) {}
               DRNetRequest* request;
               CallbackCommand* command;
           };
           std::stack<RequestCommand> mRequestCommands;
        };
    }
}

#endif //__UNIVERSUM_LIB_SERVER_CONNECTION_TO_SERVER__H