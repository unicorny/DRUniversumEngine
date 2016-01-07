#include "controller/OctreeNode.h"

namespace UniLib {
	namespace controller {


		OctreeNode::OctreeNode(OctreeNode* parent /* = NULL */)
			: mParent(parent)
		{
			memset(mChilds, 0, 4*sizeof(OctreeNode*));
		}

		OctreeNode::~OctreeNode()
		{

		}
	}
}