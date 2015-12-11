#include "controller/InstancingBlockManager.h"
#include "view/block/GeometrieBlock.h"

namespace UniLib {
	namespace controller {

		InstancingBlockManager* InstancingBlockManager::mpInstanz = NULL;
		InstancingBlockManager::InstancingBlockManager()
		{

		}

		InstancingBlockManager::~InstancingBlockManager()
		{

		}

		InstancingBlockManager* InstancingBlockManager::getInstance()
		{
			// Die Instanz wird erst beim ersten Aufruf erzeugt.
			if (!mpInstanz)
				mpInstanz = new InstancingBlockManager;
			return mpInstanz;
		}

		void InstancingBlockManager::clearEmptyGeometrieBlocks()
		{
			for(std::map<HASH, view::block::GeometrieBlock*>::iterator it = mGeometrieBlocks.begin(); it != mGeometrieBlocks.end(); it++) {
				if(it->second->getGeometrieBlockCount() == 0) {
					delete it->second;
					it = mGeometrieBlocks.erase(it);
				}
			}
			//clearEmptyGeometrieBlocks
		}

		view::block::GeometrieBlock* InstancingBlockManager::findGeometrieBlock(HASH id)
		{
			std::map<HASH, view::block::GeometrieBlock*>::iterator it = mGeometrieBlocks.find(id);
			if(it != mGeometrieBlocks.end()) return it->second;
			return NULL;
		}
	}
}