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

namespace UniLib {
    namespace server {
        class UNIVERSUM_LIB_API ConnectionToAccountServer: public ConnectionToServer
        {
        public:
            ConnectionToAccountServer();
            virtual ~ConnectionToAccountServer();

            __inline__ bool isLogin() {return mSuccesfullyLoggedIn;}
        protected:
            bool mSuccesfullyLoggedIn;
        };
    }
}



#endif //__UNIVERSUM_LIB_SERVER_CONNECTION_TO_ACCOUNT_SERVER_H