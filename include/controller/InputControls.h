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
		enum UNIVERSUM_LIB_API InputCommandEnum
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

		class InputCommand;

		class UNIVERSUM_LIB_API InputControls: public lib::Singleton
		{
		public:
			// return the one and one instance (create if not exist)
			static InputControls* getInstance();

			// \return true if key is currently pressed
			bool isKeyPressed(SDL_Keycode whichKey);
			// \return true if key is currently pressed
			bool isKeyPressed(InputCommandEnum whichKey);

			void addingInputCommand(InputCommand* cmd);
			void removeInputCommand(InputCommand* cmd);

			// set key command mapping
			void setMapping(SDL_Keycode sdlKeycode, InputCommandEnum command);

			SDL_Keycode getKeyCodeForCommand(InputCommandEnum command);
			InputCommandEnum getCommandForKeycode(SDL_Keycode sdlKeycode);

			// update event queue, must be called every frame from main thread (SDL init thread)
			DRReturn inputLoop();

			// \return return mouse move since last frame
			__inline__ DRVector2i getRelativeMousePos() {return mMouseMoveBuffer;}



		protected:
			InputControls();
			virtual ~InputControls();

			__inline__ void lock() {SDL_LockMutex(mSDLWorkMutex);}
			__inline__ void unlock() {SDL_UnlockMutex(mSDLWorkMutex);}

			// command type key code mapping
			typedef std::pair<InputCommandEnum, SDL_Keycode> CommandMappingPair;
			typedef std::map<InputCommandEnum, SDL_Keycode>::iterator CommandMappingIterator;
			std::map<InputCommandEnum, SDL_Keycode> mCommandMapping;

			// key code command mapping
			typedef std::pair<SDL_Keycode, InputCommandEnum> KeycodeMappingPair;
			typedef std::map<SDL_Keycode, InputCommandEnum>::iterator KeycodeMappingIterator;
			std::map<SDL_Keycode, InputCommandEnum> mKeycodeMapping;

			std::list<InputCommand*> mInputCommands;
			DRVector2i mMouseMoveBuffer;

			// thread save
			SDL_mutex* mSDLWorkMutex;
			Uint8*     mLastKeyStates;
			int		   mLastKeyStatesSize;

		private:
			static InputControls* mpInstanz;
		};
	}
}

#endif //_DR_UNIVERSUM_LIB_CONTROLLER_INPUT_CONTROLS_H_