#include "controller/InputCamera.h"
#include "controller/InputControls.h"

namespace UniLib {
	namespace controller {

		InputCamera::InputCamera(float mvSpeed, float rotateSpeed)
			: mMoveSpeed(mvSpeed), mRotationSpeed(rotateSpeed)
		{
			InputControls::getInstance()->addingInputCommand(this);
		}

		InputCamera::~InputCamera()
		{

		}

		DRReturn InputCamera::input(InputCommandEnum in)
		{
			return DR_OK;
		}
	}
}