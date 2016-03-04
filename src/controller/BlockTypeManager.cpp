#include "controller/BlockTypeManager.h"
#include "controller/CPUSheduler.h"
#include "model/block/MaterialBlock.h"

namespace UniLib {
	namespace controller {

		// CPU TASK 
		BlockTypeLoadingTask::BlockTypeLoadingTask(CPUSheduler* scheduler, const char* jsonFileName)
			: CPUTask(scheduler), mJsonFileName(jsonFileName)
		{

		}

		BlockTypeLoadingTask::~BlockTypeLoadingTask()
		{

		}

		DRReturn BlockTypeLoadingTask::run()
		{
			DRReturn ret = BlockTypeManager::getInstance()->_init(mJsonFileName.data());
			return ret;
			//return 
		}

		// ************************************************************************************************++
		// BLock Material Manager
		// ************************************************************************************************++

		BlockTypeManager::BlockTypeManager() 
			: mLoadingState(LOADING_STATE_EMPTY)
		{

		}

		BlockTypeManager::~BlockTypeManager()
		{
			exit();
		}

		BlockTypeManager* BlockTypeManager::getInstance()
		{
			static BlockTypeManager theOne;
			return &theOne;
		}

		DRReturn BlockTypeManager::init(const char* jsonFileName)
		{
			return _init(jsonFileName);
		}
		DRReturn BlockTypeManager::initAsyn(const char* jsonFileName, CPUSheduler* scheduler)
		{
			TaskPtr task(new BlockTypeLoadingTask(scheduler, jsonFileName));
			((CPUTask*)(task.getResourcePtrHolder()->mResource))->start(task);
			return DR_OK;
		}

		DRReturn BlockTypeManager::_init(const char* jsonFilename)
		{			
			std::string cfg = readFileAsString(jsonFilename);
			mWorkMutex.lock();
			mLoadingState = LOADING_STATE_HAS_INFORMATIONS;
			mWorkMutex.unlock();
			Json::Value json = convertStringToJson(cfg);
			Json::Value material = json.get("materialTypes", Json::Value());
			if(material.isArray() ) {
				for(int i = 0; i < material.size(); i++) {
					Json::Value entry = material[i];
					std::string name = entry.get("name", "noname").asString();
					HASH id = DRMakeStringHash(name.data());
					mWorkMutex.lock();
					BlockTypelIter it = mBlockTypes.find(id);
					if(it != mBlockTypes.end()) {
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
						mBlockTypes.insert(BlockTypePair(id, mat));
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

		model::block::BlockType* BlockTypeManager::getBlockType(HASH id)
		{
			mWorkMutex.lock();
			BlockTypelIter it = mBlockTypes.find(id);
			model::block::BlockType* result = NULL;
			if (it != mBlockTypes.end()) {
				result = it->second;
			}
			mWorkMutex.unlock();
			return result;
		}

		void BlockTypeManager::exit()
		{
			mWorkMutex.lock();
			for(BlockTypelIter it = mBlockTypes.begin(); it != mBlockTypes.end(); it++) {
				delete it->second;
			}
			mBlockTypes.clear();
			mWorkMutex.unlock();
		}
	}
}