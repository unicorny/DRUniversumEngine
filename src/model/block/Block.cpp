#include "model/block/Block.h"
#include "model/block/BlockType.h"
#include "controller/BlockTypeManager.h"

namespace UniLib {
	namespace model {
		namespace block {
			Block::Block(HASH blockTypeId)
				: mTypeData(NULL)
			{
				mTypeData = controller::BlockTypeManager::getInstance()->getBlockType(blockTypeId);
			}

			Block::Block(const char* name)
				: mTypeData(NULL)
			{
				mTypeData = controller::BlockTypeManager::getInstance()->getBlockType(name);
			}


			Block::~Block()
			{

			}
		}
	}
}