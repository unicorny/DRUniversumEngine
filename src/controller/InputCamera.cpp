#include "controller/InputCamera.h"
#include "controller/InputControls.h"
#include "controller/GPUScheduler.h"

namespace UniLib {
	namespace controller {

		InputCamera::InputCamera(float mvSpeed, float rotateSpeed, float fov)
			: Camera(fov), mMoveSpeed(mvSpeed), mRotationSpeed(rotateSpeed)
		{
			//InputControls::getInstance()->addingInputCommand(this);
		}

		InputCamera::~InputCamera()
		{

		}

		void InputCamera::updateDirectlyFromKeyboard()
		{
			InputControls* input = InputControls::getInstance();
			float t = GPUScheduler::getInstance()->getSecondsSinceLastFrame();
			const Uint8 *keys = SDL_GetKeyboardState(NULL);

			float speed = mMoveSpeed * t;
			SDL_Keycode k = input->getKeyCodeForCommand(INPUT_STRAFE_LEFT);
			Uint8 val = keys[k];
			mPosition.move(DRVector3(
				(keys[input->getKeyCodeForCommand(INPUT_STRAFE_LEFT)]-keys[input->getKeyCodeForCommand(INPUT_STRAFE_RIGHT)])*speed,
				(keys[input->getKeyCodeForCommand(INPUT_STRAFE_DOWN)]-keys[input->getKeyCodeForCommand(INPUT_STRAFE_UP)])*speed,
				(keys[input->getKeyCodeForCommand(INPUT_ACCELERATE)]-keys[input->getKeyCodeForCommand(INPUT_RETARD)])*speed));
			speed = mRotationSpeed * t;
			mRotation.rotateRel(DRVector3(
				(-keys[input->getKeyCodeForCommand(INPUT_ROTATE_UP)]+keys[input->getKeyCodeForCommand(INPUT_ROTATE_DOWN)])*speed,
				(-keys[input->getKeyCodeForCommand(INPUT_ROTATE_LEFT)]+keys[input->getKeyCodeForCommand(INPUT_ROTATE_RIGHT)])*speed,
				(-keys[input->getKeyCodeForCommand(INPUT_TILT_LEFT)]+keys[input->getKeyCodeForCommand(INPUT_TILT_RIGHT)])*speed));
		}
		DRReturn InputCamera::input(InputCommandEnum in)
		{
			float t = GPUScheduler::getInstance()->getSecondsSinceLastFrame();

			return DR_OK;
		}
	}
}