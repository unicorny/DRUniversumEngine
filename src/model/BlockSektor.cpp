#include "model/BlockSektor.h"
#include "view/BlockSector.h"

namespace UniLib {
	namespace model {

		BlockSektor::BlockSektor(view::BlockSektor* viewSektor /* = NULL */)
			: Sektor(dynamic_cast<view::Sektor*>(viewSektor))
		{
			memset(mBlockGrid, 0, sizeof(short)*9*9*9);
		}

		BlockSektor::~BlockSektor()
		{

		}

		void addBlock(block::BlockPtr block, DRVector3i index)
		{
		}
		
	}
}