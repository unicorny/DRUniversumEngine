#include "view/VisibleNode.h"

namespace UniLib {
	namespace view {
		VisibleNode::VisibleNode(Node* parent /* = NULL */, const DRVector3& position /* = DRVector3(0.0f) */)
			:MoveableNode(parent, position)
		{
			mType |= VISIBLE_NODE;
		}
	}
}