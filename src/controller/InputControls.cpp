#include "controller/InputControls.h"
#include "controller/InputCommand.h"

namespace UniLib {
	namespace controller {

		InputControls* InputControls::mpInstanz = NULL;

		InputControls::InputControls()
			: mMouseMoveBuffer(0), mSDLWorkMutex(SDL_CreateMutex()), mLastKeyStates(NULL), mLastKeyStatesSize(0)
		{
			if(!mSDLWorkMutex) LOG_WARNING_SDL();
		}

		InputControls::~InputControls()
		{
			lock();
			unlock();
			SDL_DestroyMutex(mSDLWorkMutex);
			mSDLWorkMutex = NULL;
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
			lock();
			assert(whichKey > 0 && whichKey < mLastKeyStatesSize);
			bool result = mLastKeyStates[whichKey] == 1;
			unlock();
			return result;
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
			SDL_GetRelativeMouseState(&mMouseMoveBuffer.x, &mMouseMoveBuffer.y);

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
			// copy current keyboard key state for access from other threads
			int keyCount = 0;
			const Uint8 *keys = SDL_GetKeyboardState(&keyCount);
			lock();
			if(keyCount != mLastKeyStatesSize) {
				mLastKeyStatesSize = keyCount;
				DR_SAVE_DELETE_ARRAY(mLastKeyStates);
				mLastKeyStates = new Uint8[mLastKeyStatesSize];
			}
			memcpy(mLastKeyStates, keys, sizeof(Uint8)* keyCount);
			unlock();
			return DR_OK;
		}
	}
}



