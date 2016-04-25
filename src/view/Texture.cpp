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

		DRReturn TextureSavingTask::run()
		{
			model::Texture* m = mViewTexture->getTextureModel();
			if(m->saveIntoFile(mFilename.data())) {
				LOG_ERROR("error saving texture", DR_ERROR);
			}
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
			mLoadingState = LOADING_STATE_HAS_INFORMATIONS;
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
				task->scheduleTask(task);
				//((controller::CPUTask*)(task.getResourcePtrHolder()->mResource))->start(task);
			}
			else {
				mTextureModel->loadFromFile(mTextureName.data());
				setLoadingState(LOADING_STATE_PARTLY_LOADED);
			}
		}

		void Texture::saveIntoFile(const char* filename)
		{
			if (g_HarddiskScheduler) {
				controller::TaskPtr task(new TextureSavingTask(this, g_HarddiskScheduler, filename));
				task->scheduleTask(task);
			}
			else {
				mTextureModel->saveIntoFile(filename);
			}
		}

		DRReturn Texture::loadFromMemory(u8* data)
		{
			if (!mTextureModel) 
				LOG_ERROR("texture model not created, size not known!", DR_ERROR);
			if (mTextureModel->loadFromMemory(data)) 
				LOG_ERROR("error loading from memory", DR_ERROR);
			setLoadingState(LOADING_STATE_PARTLY_LOADED);
			return DR_OK;
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