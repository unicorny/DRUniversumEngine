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
			return ret;
			//return 
		}

		// ************************************************************************************************++
		// BLock Material Manager
		// ************************************************************************************************++

		BlockMaterialManager::BlockMaterialManager() 
			: mLoadingState(LOADING_STATE_EMPTY)
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
			TaskPtr task(new MaterialsLoadingTask(scheduler, jsonFileName));
			((CPUTask*)(task.getResourcePtrHolder()->mResource))->start(task);
			return DR_OK;
		}

		DRReturn BlockMaterialManager::_init(const char* jsonFilename)
		{			
			std::string material = readFileAsString(jsonFilename);
			mWorkMutex.lock();
			mLoadingState = LOADING_STATE_HAS_INFORMATIONS;
			mWorkMutex.unlock();
			Json::Value json = convertStringToJson(material);
			if(json.isArray() ) {
				for(int i = 0; i < json.size(); i++) {
					Json::Value entry = json[i];
					std::string name = entry.get("name", "noname").asString();
					HASH id = DRMakeStringHash(name.data());
					mWorkMutex.lock();
					MaterialIter it = mMaterials.find(id);
					if(it != mMaterials.end()) {
						if(std::string(it->second->getName()) != name) {
							EngineLog.writeToLog("Material %s and Material %s have the same hash: %d",
								it->second->getName(), name.data(), id);
							mWorkMutex.unlock();
							LOG_ERROR("hash collision", DR_ERROR);
						} else {
							EngineLog.writeToLog("material: %s", name.data());
							LOG_WARNING("One material was declared more than once");
						}
					} else {
						mWorkMutex.unlock();
						model::block::MaterialBlock* mat = new model::block::MaterialBlock(name);
						if(mat->initFromJson(entry)) {
							delete mat;
							LOG_ERROR("error by init one material", DR_ERROR);
						}
						mat->setId(id);
						mWorkMutex.lock();
						mMaterials.insert(MaterialPair(id, mat));
						mLoadingState = LOADING_STATE_PARTLY_LOADED;
					}
					mWorkMutex.unlock();
				}
			} 
			else {
				LOG_ERROR("json isn't a array", DR_ERROR);
			}
			mWorkMutex.lock();
			mLoadingState = LOADING_STATE_FULLY_LOADED;
			mWorkMutex.unlock();
			return DR_OK;
		}

		model::block::MaterialBlock* BlockMaterialManager::getMaterial(HASH id)
		{
			mWorkMutex.lock();
			MaterialIter it = mMaterials.find(id);
			model::block::MaterialBlock* result = NULL;
			if (it != mMaterials.end()) {
				result = it->second;
			}
			mWorkMutex.unlock();
			return result;
		}

		void BlockMaterialManager::exit()
		{
			mWorkMutex.lock();
			for(MaterialIter it = mMaterials.begin(); it != mMaterials.end(); it++) {
				delete it->second;
			}
			mMaterials.clear();
			mWorkMutex.unlock();
		}
	}
}