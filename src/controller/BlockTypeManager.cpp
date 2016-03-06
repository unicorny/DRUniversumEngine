#include "controller/BlockTypeManager.h"
#include "controller/CPUSheduler.h"
#include "model/block/MaterialBlock.h"

namespace UniLib {
	namespace controller {

		// CPU TASK 
		// parsing block type data
		DRReturn BlockTypeLoadingTask::run()
		{
			DRReturn ret = BlockTypeManager::getInstance()->_parsingJsonToBlockTypes(mFileContents);
			DR_SAVE_DELETE(mFileContents);
			return ret;
			//return 
		}
		// loading files with config data into memory
		DRReturn LoadingJsonFilesIntoMemoryTask::run()
		{
			std::list<std::string>* fileContents = new std::list<std::string>;
			DRReturn ret = BlockTypeManager::getInstance()->_loadingFilesIntoMemory(&mFileNames, fileContents);
			if (ret == DR_OK) {
				TaskPtr task(new BlockTypeLoadingTask(mSchedulerForParser, fileContents));
				((CPUTask*)(task.getResourcePtrHolder()->mResource))->start(task);
			}
			return ret;
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
		// called from io Thread or directly
		DRReturn BlockTypeManager::init(const std::list<std::string>* fileNames)
		{
			std::list<std::string> fileContents;
			if (_loadingFilesIntoMemory(fileNames, &fileContents)) {
				LOG_ERROR("error by loading files into memory", DR_ERROR);
			}
			if (_parsingJsonToBlockTypes(&fileContents)) {
				LOG_ERROR("error by parsing Json to block types", DR_ERROR);
			}
			return DR_OK;
			
			//return _init();
		}
		DRReturn BlockTypeManager::initAsyn(const std::list<std::string>* fileNames, CPUSheduler* scheduler)
		{
			TaskPtr task(new LoadingJsonFilesIntoMemoryTask(fileNames, scheduler));
			((CPUTask*)(task.getResourcePtrHolder()->mResource))->start(task);
			return DR_OK;
		}
		// called from io Thread or callers thread
		DRReturn BlockTypeManager::_loadingFilesIntoMemory(const std::list<std::string>* fileNames, std::list<std::string>* fileContentsReturn)
		{
			assert(fileNames != NULL);
			assert(fileContentsReturn != NULL);
			Uint32 startTicks = SDL_GetTicks();

			for (std::list<std::string>::const_iterator it = fileNames->begin(); it != fileNames->end(); it++) {
				fileContentsReturn->push_back(readFileAsString(*it));
			}

			EngineLog.writeToLog("[BlockTypeManager::_loadingFilesIntoMemory] running time: %d ms",
				SDL_GetTicks() - startTicks);

			return DR_OK;
		}

		DRReturn BlockTypeManager::_parsingJsonToBlockTypes(const std::list<std::string>* mFilesContent)
		{			
			Uint32 startTicks = SDL_GetTicks();
			mWorkMutex.lock();
			mLoadingState = LOADING_STATE_HAS_INFORMATIONS;
			mWorkMutex.unlock();
			for (std::list<std::string>::const_iterator it = mFilesContent->begin(); it != mFilesContent->end(); it++)
			{
				Json::Value json = convertStringToJson(*it);
				Json::Value material = json.get("materialTypes", Json::Value());
				if (material.isArray()) {
					for (int i = 0; i < material.size(); i++) {
						Json::Value entry = material[i];
						std::string name = entry.get("name", "noname").asString();
						HASH id = DRMakeStringHash(name.data());
						mWorkMutex.lock();
						BlockTypelIter it = mBlockTypes.find(id);
						if (it != mBlockTypes.end()) {
							if (std::string(it->second->getName()) != name) {
								EngineLog.writeToLog("Material %s and Material %s have the same hash: %d",
									it->second->getName(), name.data(), id);
								mWorkMutex.unlock();
								LOG_ERROR("hash collision", DR_ERROR);
							}
							else {
								EngineLog.writeToLog("material: %s", name.data());
								LOG_WARNING("One material was declared more than once");
							}
						}
						else {
							mWorkMutex.unlock();
							model::block::MaterialBlock* mat = new model::block::MaterialBlock(name);
							if (mat->initFromJson(entry)) {
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
			}
			mWorkMutex.lock();
			mLoadingState = LOADING_STATE_FULLY_LOADED;
			mWorkMutex.unlock();
			EngineLog.writeToLog("[BlockTypeManager::_parsingJsonToBlockTypes] running time: %d ms", SDL_GetTicks() - startTicks);
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