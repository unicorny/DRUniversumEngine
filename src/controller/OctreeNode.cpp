#include "controller/OctreeNode.h"
#include "model/SektorID.h"

namespace UniLib {
	namespace controller {


		OctreeNode::OctreeNode(model::SektorID* id, model::Node* parent /* = NULL */)
			: Sektor(parent, id)
		{
			mType |= OCTREE_NODE;
			memset(mChilds, 0, sizeof(model::Node*) * 8);
		}

		OctreeNode::~OctreeNode()
		{

		}

		DRReturn OctreeNode::move(float timeSinceLastFrame)
		{
			for (int i = 0; i < 8; i++) {
				if (mChilds[i]) mChilds[i]->move(timeSinceLastFrame);
			}
			return DR_OK;
		}
	}
}