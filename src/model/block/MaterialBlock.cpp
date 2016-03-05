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
				if(mLoadingState == LOADING_STATE_EMPTY) {
					std::string baseType = json.get("base_type", "solid").asString();
					mBaseType = getBlockBaseTypeEnum(baseType);
					mTransparency = json.get("transparency", false).asBool();
					mDensity = json.get("density", 0.0f).asFloat();
					mMeltingPoint = json.get("melting_point", 0.0f).asFloat();
					mHitpoints = json.get("hitpoints", 0).asInt();
					Json::Value shaderJson = json.get("shader", Json::Value());
					mFragmentShaderName = shaderJson.get("fragment", std::string("")).asString();
					mVertexShaderName = shaderJson.get("vertex", std::string("")).asString();
					mLoadingState = LOADING_STATE_FULLY_LOADED;

					//EngineLog.writeToLog("[MaterialBlock::initFromJson] adding block: %s", asString().data());
				}
				unlock();

				return DR_OK;
			}


		}
	}
}