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

/*!
 * \author Dario Rekowski
 * 
 * \date 25.10.2015
 * 
 * \desc InputControls Singleton, Input Control Hub
 */

#ifndef _DR_UNIVERSUM_LIB_CONTROLLER_INPUT_CONTROLS_H_
#define _DR_UNIVERSUM_LIB_CONTROLLER_INPUT_CONTROLS_H_

#include "lib/Singleton.h"

namespace UniLib {
	namespace controller {

		// Binary Commands 
		enum UNIVERSUM_LIB_API InputBinaryCommandEnum
		{
			INPUT_UNKNOWN = 0,
			INPUT_ROTATE_LEFT = 1,
			INPUT_ROTATE_RIGHT = 3,
			INPUT_ROTATE_UP = 4,
			INPUT_ROTATE_DOWN = 5,
			INPUT_TILT_LEFT = 6,
			INPUT_TILT_RIGHT = 7,
			INPUT_STRAFE_LEFT = 8,
			INPUT_STRAFE_RIGHT = 9,
			INPUT_STRAFE_UP = 10,
			INPUT_STRAFE_DOWN = 11,
			INPUT_ACCELERATE = 12,
			// verzögern, pull down speed
			INPUT_RETARD = 13,
			INPUT_ATTACK_1 = 14,
			INPUT_ATTACK_2 = 15,
			INPUT_OPEN_MAIN_MENU = 16,
			INPUT_JUMP = 17,
			INPUT_PRIMARY_ACTION = 18,
			INPUT_SECONDARY_ACTION = 19,
			INPUT_LEFT_MOUSE_CLICK = 20,
			INPUT_RIGHT_MOUSE_CLICK = 21,
			INPUT_MIDDLE_MOUSE_CLICK = 22
		};

		// Input with number (mouse and joystick movement)
		enum UNIVERSUM_LIB_API InputNumberCommandEnum
		{
			MOUSE_UNKNOWN = 0,
			MOUSE_AXIS_X = 1,
			MOUSE_AXIS_Y = 2,
			MOUSE_AXIS_Z = 3
		};

		class UNIVERSUM_LIB_API InputControls: public lib::Singleton
		{
		public:
			// return the one and one instance (create if not exist)
			static InputControls* getInstance();

			// \return true if key is currently pressed
			bool isKeyPressed(SDL_Keycode whichKey);
			// \return true if key is currently pressed
			bool isKeyPressed(InputBinaryCommandEnum whichKey);

			// set key command mapping
			void setMapping(SDL_Keycode sdlKeycode, InputBinaryCommandEnum command);
			void setMapping(SDL_Keycode sdlKeycode, InputNumberCommandEnum command);

			SDL_Keycode getKeyCodeForCommand(InputBinaryCommandEnum command);
			SDL_Keycode getKeyCodeForCommand(InputNumberCommandEnum command);
			InputBinaryCommandEnum getBinaryCommandForKeycode(SDL_Keycode sdlKeycode);
			InputNumberCommandEnum getNumberCommandForKeycode(SDL_Keycode sdlKeycode);

			// update event queue
			DRReturn inputLoop();

		protected:
			InputControls();
			virtual ~InputControls();

			// command type key code mapping
			typedef std::pair<InputBinaryCommandEnum, SDL_Keycode> CommandBinaryMappingPair;
			typedef std::map<InputBinaryCommandEnum, SDL_Keycode>::iterator CommandBinaryMappingIterator;
			std::map<InputBinaryCommandEnum, SDL_Keycode> mCommandBinaryMapping;
			typedef std::pair<InputNumberCommandEnum, SDL_Keycode> CommandNumberMappingPair;
			typedef std::map<InputNumberCommandEnum, SDL_Keycode>::iterator CommandNumberMappingIterator;
			std::map<InputNumberCommandEnum, SDL_Keycode> mCommandNumberMapping;

			// key code command mapping
			typedef std::pair<SDL_Keycode, InputBinaryCommandEnum> KeycodeBinaryMappingPair;
			typedef std::map<SDL_Keycode, InputBinaryCommandEnum>::iterator KeycodeBinaryMappingIterator;
			std::map<SDL_Keycode, InputBinaryCommandEnum> mKeycodeBinaryMapping;
			typedef std::pair<SDL_Keycode, InputNumberCommandEnum> KeycodeNumberMappingPair;
			typedef std::map<SDL_Keycode, InputNumberCommandEnum>::iterator KeycodeNumberMappingIterator;
			std::map<SDL_Keycode, InputNumberCommandEnum> mKeycodeNumberMapping;
		};
	}
}

#endif //_DR_UNIVERSUM_LIB_CONTROLLER_INPUT_CONTROLS_H_