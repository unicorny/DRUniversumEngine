#include "model/block/MaterialBlock.h"

namespace UniLib {
	namespace model {
		namespace block {
			MaterialBlock::MaterialBlock(std::string name)
			: BlockType(name)
			{
				
			}

			MaterialBlock::~MaterialBlock() 
			{

			}

			DRReturn MaterialBlock::initFromJson(const Json::Value& json)
			{
				lock();
				if(mLoadingState == BLOCK_TYPE_EMPTY) {
					const char* baseType = json.get("base_type", "solid").asCString();
					mBaseType = getBlockBaseTypeEnum(baseType);
					mTransparency = json.get("transparency", false).asBool();
					mDensity = json.get("density", 0.0f).asFloat();
					mMeltingPoint = json.get("melting_point", 0.0f).asFloat();
					mHitpoints = json.get("hitpoints", 0).asInt();
					Json::Value shaderJson = json.get("shader", Json::Value());
				}
				unlock();

				return DR_OK;
			}


		}
	}
}