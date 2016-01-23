#include "controller/BlockMaterialManager.h"
#include "controller/CPUSheduler.h"
#include "model/block/MaterialBlock.h"

namespace UniLib {
	namespace controller {

		// CPU TASK 
		MaterialsLoadingTask::MaterialsLoadingTask(CPUSheduler* scheduler, const char* jsonFileName)
			: CPUTask(scheduler), mJsonFileName(jsonFileName)
		{

		}

		MaterialsLoadingTask::~MaterialsLoadingTask()
		{

		}

		DRReturn MaterialsLoadingTask::run()
		{
			DRReturn ret = BlockMaterialManager::getInstance()->_init(mJsonFileName.data());
			delete this;
			return ret;
			//return 
		}

		// ************************************************************************************************++
		// BLock Material Manager
		// ************************************************************************************************++

		BlockMaterialManager::BlockMaterialManager() 
		{

		}

		BlockMaterialManager::~BlockMaterialManager()
		{
			exit();
		}

		BlockMaterialManager* BlockMaterialManager::getInstance()
		{
			static BlockMaterialManager theOne;
			return &theOne;
		}

		DRReturn BlockMaterialManager::init(const char* jsonFileName)
		{
			return _init(jsonFileName);
		}
		DRReturn BlockMaterialManager::initAsyn(const char* jsonFileName, CPUSheduler* scheduler)
		{
			new MaterialsLoadingTask(scheduler, jsonFileName);
			return DR_OK;
		}

		DRReturn BlockMaterialManager::_init(const char* jsonFilename)
		{
			mWorkMutex.lock();
			std::string material = readFileAsString(jsonFilename);
			Json::Value json = convertStringToJson(material);
			if(json.isArray() ) {
				for(int i = 0; i < json.size(); i++) {
					Json::Value entry = json[i];
					std::string name = entry.get("name", "noname").asString();
					HASH id = DRMakeStringHash(name.data());
					MaterialIter it = mMaterials.find(id);
					if(it != mMaterials.end()) {
						if(std::string(it->second->getName()) != name) {
							EngineLog.writeToLog("Material %s and Material %s have the same hash: %d",
								it->second->getName(), name.data(), id);
							LOG_ERROR("hash collision", DR_ERROR);
						} else {
							EngineLog.writeToLog("material: %s", name.data());
							LOG_WARNING("One material was declared more than once");
						}
					} else {
						model::block::MaterialBlock* mat = new model::block::MaterialBlock(name);
						if(mat->initFromJson(json)) {
							delete mat;
							mWorkMutex.unlock();
							LOG_ERROR("error by init one material", DR_ERROR);
						}
						mat->setId(id);
						mMaterials.insert(MaterialPair(id, mat));
					}
				}
			} 
			else {
			}
			mWorkMutex.unlock();
			return DR_OK;
		}

		void BlockMaterialManager::exit()
		{
			for(MaterialIter it = mMaterials.begin(); it != mMaterials.end(); it++) {
				delete it->second;
			}
			mMaterials.clear();
		}
	}
}