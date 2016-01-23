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

#define _UNI_LIB_DEBUG

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif //_WIN32



#include "Core2/Core2Main.h"
#include "sdl/SDL.h"
#include "lib/Logging.h"
#include "controller/BindToRenderer.h"
#include "json/json.h"

#include <queue>
#include <cassert>

namespace UniLib {
    UNIVERSUM_LIB_API extern lib::EngineLogger EngineLog;
	extern controller::BindToRenderer* g_RenderBinder;
}

#undef WRITETOLOG
#undef LOG_ERROR
#undef LOG_ERROR_VOID
#undef LOG_INFO
#undef LOG_WARNING

//makros für Log Benutzung
#define WRITETOLOG(str, pl) UniLib::EngineLog.writeToLog(str, pl);
//#define DR_ERROR(str, ret) {DRLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR(str, ret) {UniLib::EngineLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR_VOID(str) {UniLib::EngineLog.LOG_ERROR_INTERN(str); return;}
#define LOG_INFO(str) UniLib::EngineLog.LOG_INFO_INTERN(str);
#define LOG_WARNING(str) UniLib::EngineLog.LOG_WARNING_INTERN(str);
//*/

#define LOG_SDL_INTERN(text, f, l, fu) DRLog.writeToLog("<tr><td><font size=\"2\"><b><font color=\"#BF8000\">SDL Fehler:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text, f, l, fu)
#define LOG_ERROR_SDL(r) {const char* pcErrorSDL = SDL_GetError(); if(strlen(pcErrorSDL) > 2){ LOG_SDL_INTERN(pcErrorSDL, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__); return r;}}
#define LOG_ERROR_SDL_VOID() {const char* pcErrorSDL = SDL_GetError(); if(strlen(pcErrorSDL) > 2){ LOG_SDL_INTERN(pcErrorSDL, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__); return;}}
#define LOG_WARNING_SDL() {const char* pcErrorSDL = SDL_GetError(); if(strlen(pcErrorSDL) > 2) LOG_SDL_INTERN(pcErrorSDL, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__);}


#ifndef __inline__
#define  __inline__ inline
#endif



// engine functions
namespace UniLib {	
	// node types
	enum UNIVERSUM_LIB_API NodeTypes {
		NODE_NONE = 0,
		NODE = 1,
		MOVEABLE_NODE = 2,
		VISIBLE_NODE = 4
	};

    UNIVERSUM_LIB_API DRReturn init();
    UNIVERSUM_LIB_API void exit();

    UNIVERSUM_LIB_API DRString getTimeSinceStart();
	UNIVERSUM_LIB_API std::string readFileAsString(std::string filename);
	UNIVERSUM_LIB_API Json::Value convertStringToJson(std::string jsonString);
    UNIVERSUM_LIB_API DRString getValueAsBinaryString(u8 zahl);
	UNIVERSUM_LIB_API DRString getValueAsBinaryString(int zahl);

	UNIVERSUM_LIB_API void setBindToRenderer(controller::BindToRenderer* bindToRender);
}


#endif //__DR_UNIVERSUM_LIB_MAIN_H__