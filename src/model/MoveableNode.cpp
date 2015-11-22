#include "model/MoveableNode.h"

namespace UniLib {
	namespace model {

		MoveableNode::MoveableNode(Node* parent /* = NULL */, const DRVector3& position /* = DRVector3(0.0f) */)
			: Node(parent), mPosition(position)
		{
			mType |= MOVEABLE_NODE;
		}

		void MoveableNode::calculateMatrix()
		{
			mMatrix = mPosition.calculateMatrix(mMatrix) * mRotation.calculateMatrix();
			if(mParent && (mParent->getType() & MOVEABLE_NODE) != 0) {
				MoveableNode* parent = dynamic_cast<MoveableNode*>(mParent);
				mMatrix *= parent->getMatrix();
			}
			for(std::list<Node*>::iterator it = mChilds.begin(); it != mChilds.end(); it++) {
				if(((*it)->getType() & MOVEABLE_NODE) != 0) {
					MoveableNode* child = dynamic_cast<MoveableNode*>(*it);
					child->calculateMatrix();
				}
			}
		}
	}
}