#include "controller/BlockSektorTree.h"
#include "controller/BindToRenderer.h"
#include "model/BlockSektor.h"
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

		DRReturn BlockSektorTree::addBlock(model::block::BlockPtr block, std::queue<u8> path, DRVector3i pos)
		{
			assert(!path.empty());
			u8 index = path.front();
			path.pop();
			model::Node* node = mChilds[index];
			// we have no node on this place, we must create it
			if (!node) {
				if (!path.empty()) {
					model::SektorID* id = new model::SektorID(index, model::SEKTOR_ID_BLOCK_SEKTOR_TREE);
					id->setParent(mID);
					BlockSektorTree* n = new BlockSektorTree(id, this);
					mChilds[index] = n;
					return n->addBlock(block, path, pos);
				}
				else {
					model::SektorID* id = new model::SektorID(index, model::SEKTOR_ID_BLOCK_SEKTOR);
					id->setParent(mID);
					model::BlockSektor* b = new model::BlockSektor(this, id, g_RenderBinder->newBlockSektor());
					mChilds[index] = b;
					return b->addBlock(block, pos);
				}
			}
			// the leaf is not yet reached
			else if (node->isTypeOf((NodeTypes)mType) ) {
				BlockSektorTree* n = static_cast<BlockSektorTree*>(node);
				if (!path.empty()) {
					return n->addBlock(block, path, pos);
				}
			}
			// we actually have the leaf node
			else if (node->isTypeOf(BLOCK_SEKTOR_NODE)) {
				model::BlockSektor* b = static_cast<model::BlockSektor*>(node);
				return b->addBlock(block, pos);
			}
			else {
				LOG_ERROR("node has wrong type", DR_ERROR);
			}
			LOG_ERROR("didn't found valid path", DR_ERROR);
		}
	}
}