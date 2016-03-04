#include "controller/BlockSektorTree.h"
#include "model/SektorID.h"

namespace UniLib {
	namespace controller {
		BlockSektorTree::BlockSektorTree(model::SektorID* id, model::Node* parent)
			: OctreeNode(id, parent)
		{
			mType |= BLOCK_SECTOR_TREE_NODE;
		}

		BlockSektorTree::~BlockSektorTree()
		{

		}

		DRReturn BlockSektorTree::move(float timeSinceLastFrame)
		{
			return OctreeNode::move(timeSinceLastFrame);
			//return DR_OK;
		}

		void BlockSektorTree::addBlock(model::block::BlockPtr block, std::queue<u8> path, DRVector3i pos)
		{
			assert(!path.empty());
			u8 index = path.front();
			path.pop();
			model::Node* node = mChilds[index];
			if (node->isTypeOf((NodeTypes)mType) && !path.empty()) {
				BlockSektorTree* n = dynamic_cast<BlockSektorTree*>(node);
				
			}
		}
	}
}