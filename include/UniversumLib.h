/*/*************************************************************************
 *                                                                         *
 * UniversumLib, collection of classes for generating and go through a     *
 * whole universe. It is for my Gameproject Spacecraft					   * 
 * Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
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

#ifndef __DR_UNIVERSUM_LIB_MAIN_H__
#define __DR_UNIVERSUM_LIB_MAIN_H__

#ifdef _WIN32
    #ifndef BUILD_UNIVERSUM_LIB_DLL
        #define UNIVERSUM_LIB_API __declspec(dllimport)
    #else
        #define UNIVERSUM_LIB_API __declspec(dllexport)
    #endif
#else //_WIN32
    #ifdef BUILD_UNIVERSUM_LIB_DLL
        #define UNIVERSUM_LIB_API
    #else
        #define UNIVERSUM_LIB_API
    #endif
#endif //_WIN32


#include "../../thirdparty/include/Core2/Core2Main.h"
#include "server/Server.h"

#include "model/Unit.h"
#include "view/Camera.h"

#include "view/Sektor.h"
#include "model/Sektor.h"
#include "generator/Sektor.h"

#include "controller/Sektor.h"
#include "model/Planet.h"
#include "generator/Planet.h"
#include "view/Planet.h"
#include "controller/Planet.h"



#ifndef __inline__
#define  __inline__ inline
#endif



#endif //__DR_UNIVERSUM_LIB_MAIN_H__