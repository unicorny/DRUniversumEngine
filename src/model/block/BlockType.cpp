#include "model/block/BlockType.h"

namespace UniLib {
	namespace model {
		namespace block {

			BlockType::BlockType(std::string name)
				: mName(name), mLoadingState(BLOCK_TYPE_EMPTY)
			{

			}

			BlockType::~BlockType()
			{

			}

			BlockBaseType BlockType::getBlockBaseTypeEnum(std::string blockBaseTypeString)
			{
				if(blockBaseTypeString == std::string("solid")) return BLOCK_BASE_TYPE_SOLID;
				if(blockBaseTypeString == std::string("fluent")) return BLOCK_BASE_TYPE_FLUENT;
				if(blockBaseTypeString == std::string("gas")) return BLOCK_BASE_TYPE_GAS;
				return BLOCK_BASE_TYPE_SOLID;
			}
		}
	}
}