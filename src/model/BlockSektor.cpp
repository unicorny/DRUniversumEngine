#include "model/BlockSektor.h"
#include "view/BlockSektor.h"
#include "model/block/Block.h"
#include "model/block/BlockType.h"
#include "model/SektorID.h"

namespace UniLib {
	namespace model {

		BlockSektor::BlockSektor(Node* parent, model::SektorID* id, view::BlockSektor* viewSektor)
			: Sektor(parent, id, dynamic_cast<view::Sektor*>(viewSektor))
		{
			mType = BLOCK_SEKTOR_NODE;
			memset(mGridSolid, 0, sizeof(u16) * 8 * 8);
		}			//


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

			block::BlockBaseType type = block->getBlockType()->getBaseType();
			assert(type < 3 && type >= 0);
			// logging 
			//EngineLog.writeToLog("grid before adding block: %s", getValueAsBinaryString(mGridSolid[index.y][index.z]).data());
			//clear
			mGridSolid[index.y][index.z] &= 0xffff ^ (3 << (index.x * 2));
			// set new value
			mGridSolid[index.y][index.z] |= type << (index.x * 2);
			//EngineLog.writeToLog("grid after adding this block base type: %d\n%s",
			//	(int)type, getValueAsBinaryString(mGridSolid[index.y][index.z]).data());
			

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


		DRReturn BlockSektor::move(float timeSinceLastFrame)
		{
			return DR_OK;
		}
		
	}
}