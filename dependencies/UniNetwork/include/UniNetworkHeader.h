/*/*************************************************************************
 *                                                                         *
 * UniNetwork, Library for Netwerk Communication for my					   *
 * Gameproject Spacecraft												   *			 
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


#ifdef _WIN32
#include <windows.h>
#else
#endif

#include <UniversumLib.h> 

#ifdef _WIN32
    #ifdef BUILD_DLL_UNI_NETWORK
        #define UNI_NETWORK_API __declspec(dllexport)
    #else
        #define UNI_NETWORK_API __declspec(dllimport)
    #endif
#else
    #define UNI_NETWORK_API
#endif


#include "Connection.h"
#include "ConnectionFactory.h"
#include "UniNetwork.h"

#ifdef __cplusplus
extern "C"
{
#endif

UNI_NETWORK_API DRINetwork* getInstance();

#ifdef __cplusplus
}
#endif

