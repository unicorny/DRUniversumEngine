#include "view/Object.h"

namespace UniLib {
	namespace view {

		Object::Object(model::Object* parent /* = NULL*/, DRVector3 pos /* = DRVector3(0.0f) */, DRVector3 scale /* = DRVector3(0.0f) */)
			: model::Object(parent, pos, scale)
		{

		}

		Object::~Object()
		{

		}

	}
}