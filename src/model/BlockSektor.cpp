#include "model/BlockSektor.h"
#include "view/BlockSector.h"

namespace UniLib {
	namespace model {

		BlockSektor::BlockSektor(view::BlockSektor* viewSektor /* = NULL */)
			: Sektor(dynamic_cast<view::Sektor*>(viewSektor))
		{
			//memset(mBlockGrid, 0, sizeof(short)*9*9*9);
		}

		BlockSektor::~BlockSektor()
		{

		}

		DRReturn BlockSektor::addBlock(block::BlockPtr block, DRVector3i index)
		{
			HASH h = DRMakeSmallVector3DHash(index);
			//if(isPlaceFree(index)
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
		
	}
}