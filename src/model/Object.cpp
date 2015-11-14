#include "model/Object.h"

namespace UniLib {
	namespace model {
		Object::Object(Object* parent/* = NULL*/, DRVector3 pos /* = DRVector3(0.0f) */, DRVector3 scale /* = DRVector3(0.0f) */)
			: mParent(parent), mPosition(pos), mScale(scale)
		{

		}

		Object::~Object()
		{

		}
	}
}