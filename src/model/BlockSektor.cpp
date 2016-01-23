#include "model/BlockSektor.h"
#include "view/BlockSector.h"

namespace UniLib {
	namespace model {

		BlockSektor::BlockSektor(view::BlockSektor* viewSektor)
			: Sektor(dynamic_cast<view::Sektor*>(viewSektor))
		{
		}			//memset(mBlockGrid, 0, sizeof(short)*9*9*9);


		BlockSektor::~BlockSektor()
		{

		}

		DRReturn BlockSektor::addBlock(block::BlockPtr block, DRVector3i index)
		{
			assert(index.x >= 0 && index.x < 8);
			assert(index.y >= 0 && index.y < 8);
			assert(index.z >= 0 && index.z < 8);
			HASH h = DRMakeSmallVector3DHash(index);
			if(!isPlaceFree(h)) LOG_ERROR("cannot insert new block, position already occupied!", DR_ERROR);
			mBlocks.insert(BlockPair(h, block));
			//block->
			return DR_OK;
		}

		block::BlockPtr BlockSektor::deleteBlock(DRVector3i index)
		{
			HASH h = DRMakeSmallVector3DHash(index);
			if(isPlaceFree(h)) LOG_ERROR("cannot delete block, position is empty", block::BlockPtr());
			BlockIterator it = mBlocks.find(h);
			block::BlockPtr block = it->second;
			mBlocks.erase(it);
			return block;
		}

		block::BlockPtr BlockSektor::operator[] (HASH h) const 
		{	
			BlockIterator it = mBlocks.find(h);
			if(it != mBlocks.end()) {
				return it->second;
			} else {
				return block::BlockPtr();
			}
		}

		DRReturn BlockSektor::updateVisibility(view::Camera* camera)
		{
			return DR_OK;
		}

		DRReturn BlockSektor::move(float timeSinceLastFrame)
		{
			return DR_OK;
		}
		
	}
}