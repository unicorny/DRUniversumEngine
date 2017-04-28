#include "model/MovingNode.h"

namespace UniLib {
	namespace model {

		MovingNode::MovingNode(MovingNode* parent /* = NULL */, const DRVector3& position /* = DRVector3(0.0f) */)
			: MoveableNode(parent, position) { }


		DRReturn MovingNode::move(float timeSinceLastFrame)
		{
			mPosition.move(timeSinceLastFrame * mVelocity);
			mRotation.rotateRel(timeSinceLastFrame * mRotationVelocity);
			calculateMatrix();
			return DR_OK;
		}
	}
}