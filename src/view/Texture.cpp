#include "view/Texture.h"
#include "model/Texture.h"

namespace UniLib {
	namespace view {

		// ********************************************************************
		// tasks
		DRReturn TextureLoadingTask::run()
		{
			model::Texture* m = mViewTexture->getTextureModel();
			if (m->loadFromFile(mViewTexture->getFilename().data())) {
				LOG_ERROR("error loading texture", DR_ERROR);
			}
			mViewTexture->setLoadingState(LOADING_STATE_PARTLY_LOADED);
			return DR_OK;
		}

		// ********************************************************************


		Texture::Texture(DHASH id, const char* textureName/* = NULL*/)
			: mTextureModel(NULL), mId(id)
		{
			if (textureName) {
				mTextureName = textureName;
				mLoadingState = LOADING_STATE_HAS_INFORMATIONS;
			}
		}

		Texture::Texture(DRVector2i size, GLenum format)
			: mTextureModel(new model::Texture(size, format))
		{
			mId = mTextureModel->getHash();
		}

		Texture::~Texture()
		{
			DR_SAVE_DELETE(mTextureModel);
		}

		void Texture::loadFromFile()
		{
			if (!mTextureModel) mTextureModel = new model::Texture();
			if (g_HarddiskScheduler) {
				controller::TaskPtr task(new TextureLoadingTask(this, g_HarddiskScheduler));
				((controller::CPUTask*)(task.getResourcePtrHolder()->mResource))->start(task);
			}
			else {
				mTextureModel->loadFromFile(mTextureName.data());
				setLoadingState(LOADING_STATE_PARTLY_LOADED);
			}
		}


		void Texture::setLoadingState(LoadingState state)
		{
			MultithreadResource::setLoadingState(state);
			if (state == LOADING_STATE_PARTLY_LOADED) {
				// file loaded, now we can upload to GPU
				uploadToGPU();
			}
			
		}
	}
}