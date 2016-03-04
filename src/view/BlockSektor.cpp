#include "model/BlockSektor.h"
#include "view/BlockSektor.h"

namespace UniLib {
	namespace view {

		BlockSektor::BlockSektor(model::BlockSektor* viewSektor /* = NULL */)
			: Sektor(dynamic_cast<model::Sektor*>(viewSektor))
		{

		}

		BlockSektor::~BlockSektor()
		{

		}


		DRReturn BlockSektor::updateVisibility(view::Camera* camera)
		{
			return DR_OK;
		}

		DRReturn BlockSektor::render(view::Camera* camera, float timeSinceLastFrame)
		{
			return DR_OK;
		}
	}
}