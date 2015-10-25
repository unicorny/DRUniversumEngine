#include "controller/InputControls.h"

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
		bool InputControls::isKeyPressed(InputBinaryCommandEnum whichKey) 
		{
			return isKeyPressed(getKeyCodeForCommand(whichKey));
		}
		
		void InputControls::setMapping(SDL_Keycode sdlKeycode, InputBinaryCommandEnum command)
		{
			CommandBinaryMappingIterator it = mCommandBinaryMapping.find(command);
			if(it != mCommandBinaryMapping.end()) {
				it->second = sdlKeycode;
			} else {
				mCommandBinaryMapping.insert(CommandBinaryMappingPair(command, sdlKeycode));
			}
			KeycodeBinaryMappingIterator it2 = mKeycodeBinaryMapping.find(sdlKeycode);
			if(it2 != mKeycodeBinaryMapping.end()) {
				it2->second = command;
			} else {
				mKeycodeBinaryMapping.insert(KeycodeBinaryMappingPair(sdlKeycode, command));
			}
		}
		void InputControls::setMapping(SDL_Keycode sdlKeycode, InputNumberCommandEnum command)
		{
			CommandNumberMappingIterator it = mCommandNumberMapping.find(command);
			if(it != mCommandNumberMapping.end()) {
				it->second = sdlKeycode;
			} else {
				mCommandNumberMapping.insert(CommandNumberMappingPair(command, sdlKeycode));
			}
			KeycodeNumberMappingIterator it2 = mKeycodeNumberMapping.find(command);
			if(it2 != mKeycodeNumberMapping.end()) {
				it2->second = command;
			} else {
				mKeycodeNumberMapping.insert(KeycodeNumberMappingPair(sdlKeycode, command));
			}
		}

		SDL_Keycode InputControls::getKeyCodeForCommand(InputBinaryCommandEnum command)
		{
			CommandBinaryMappingIterator it = mCommandBinaryMapping.find(command);
			if(it != mCommandBinaryMapping.end()) {
				return it->second;
			} 
			return SDLK_UNKNOWN;
		}
		SDL_Keycode InputControls::getKeyCodeForCommand(InputNumberCommandEnum command)
		{
			CommandNumberMappingIterator it = mCommandNumberMapping.find(command);
			if(it != mCommandNumberMapping.end()) {
				return it->second;
			}
			return SDLK_UNKNOWN;
		}

		InputBinaryCommandEnum InputControls::getBinaryCommandForKeycode(SDL_Keycode sdlKeycode)
		{
			KeycodeBinaryMappingIterator it = mKeycodeBinaryMapping.find(sdlKeycode);
			if(it != mKeycodeBinaryMapping.end()) {
				return it->second;
			} 
			return INPUT_UNKNOWN;
		}
		InputNumberCommandEnum InputControls::getNumberCommandForKeycode(SDL_Keycode sdlKeycode)
		{
			KeycodeNumberMappingIterator it = mKeycodeNumberMapping.find(sdlKeycode);
			if(it != mKeycodeNumberMapping.end()) {
				return it->second;
			}
			return MOUSE_UNKNOWN;
		}

		DRReturn InputControls::inputLoop()
		{
			SDL_Event event;
			/* Poll for events */
			while( SDL_PollEvent( &event ) ){
				InputBinaryCommandEnum in = getBinaryCommandForKeycode(event.type);
				if(in == INPUT_UNKNOWN) {

				}				
			}
			return DR_OK;
		}
	}
}



