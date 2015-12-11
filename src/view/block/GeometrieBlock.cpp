#include "view/block/GeometrieBlock.h"
#include "model/block/GeometrieBlock.h"

namespace UniLib {
	namespace view {
		namespace block {

			GeometrieBlock::GeometrieBlock()
				: mMaterial(NULL), mGeometrie(NULL), mMatrixUniforms(NULL)
			{

			}

			GeometrieBlock::~GeometrieBlock()
			{

			}

			void GeometrieBlock::clearGeometrieBlockList()
			{
				for(std::list<model::GeometrieBlockPtr>::iterator it = mGeometrieBlockModels.begin(); it != mGeometrieBlockModels.end(); it++) {
					if(it->getResourcePtrHolder()->getRefCount() == 1) {
						it = mGeometrieBlockModels.erase(it);
					}
				}
			}
		}
	}
}