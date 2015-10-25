#include "controller/InputControls.h"
#include "controller/InputCommand.h"

namespace UniLib {
	namespace controller {
		InputControls::InputControls()
		{

		}

		InputControls::~InputControls()
		{

		}

		InputControls* InputControls::getInstance()
		{
			// Die Instanz wird erst beim ersten Aufruf erzeugt.
			if (!mpInstanz)
				mpInstanz = new InputControls;
			return (InputControls*)mpInstanz;
		}

		bool InputControls::isKeyPressed(SDL_Keycode whichKey) 
		{
			int keyCount = 0;
			const Uint8 *keys = SDL_GetKeyboardState(&keyCount);
			assert(whichKey > 0 && whichKey < keyCount);
			return keys[whichKey] == 1;
		}
		bool InputControls::isKeyPressed(InputCommandEnum whichKey) 
		{
			return isKeyPressed(getKeyCodeForCommand(whichKey));
		}
		
		void InputControls::setMapping(SDL_Keycode sdlKeycode, InputCommandEnum command)
		{
			CommandMappingIterator it = mCommandMapping.find(command);
			if(it != mCommandMapping.end()) {
				it->second = sdlKeycode;
			} else {
				mCommandMapping.insert(CommandMappingPair(command, sdlKeycode));
			}
			KeycodeMappingIterator it2 = mKeycodeMapping.find(sdlKeycode);
			if(it2 != mKeycodeMapping.end()) {
				it2->second = command;
			} else {
				mKeycodeMapping.insert(KeycodeMappingPair(sdlKeycode, command));
			}
		}
		
		SDL_Keycode InputControls::getKeyCodeForCommand(InputCommandEnum command)
		{
			CommandMappingIterator it = mCommandMapping.find(command);
			if(it != mCommandMapping.end()) {
				return it->second;
			} 
			return SDLK_UNKNOWN;
		}
		

		InputCommandEnum InputControls::getCommandForKeycode(SDL_Keycode sdlKeycode)
		{
			KeycodeMappingIterator it = mKeycodeMapping.find(sdlKeycode);
			if(it != mKeycodeMapping.end()) {
				return it->second;
			} 
			return INPUT_UNKNOWN;
		}
		
		void InputControls::addingInputCommand(InputCommand* cmd)
		{
			mInputCommands.push_back(cmd);
		}
		void InputControls::removeInputCommand(InputCommand* cmd)
		{
			for(std::list<InputCommand*>::iterator it = mInputCommands.begin(); it != mInputCommands.end(); it++) {
				if(cmd == *it) it = mInputCommands.erase(it);
			}
		}
		DRReturn InputControls::inputLoop()
		{
			SDL_Event event;
			/* Poll for events */
			while( SDL_PollEvent( &event ) ){
				InputCommandEnum in = getCommandForKeycode(event.type);
				if(in != INPUT_UNKNOWN) {
					for(std::list<InputCommand*>::iterator it = mInputCommands.begin(); it != mInputCommands.end(); it++) 
					{
						if((*it)->input(in)) it = mInputCommands.erase(it);
					}
				}				
			}
			return DR_OK;
		}
	}
}



