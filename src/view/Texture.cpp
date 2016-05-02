#include "view/Texture.h"
#include "model/Texture.h"
#include "controller/Command.h"
//#include "controller/CPUSheduler.h"

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

		DRReturn TextureGetPixelTask::run()
		{
			return DR_OK;
		}

		DRReturn TextureSavingTask::run()
		{
			model::Texture* m = mViewTexture->getTextureModel();
			if(m->saveIntoFile(mFilename.data())) {
				LOG_ERROR("error saving texture", DR_ERROR);
			}
			if (mHasParent) mViewTexture->setLoadingState(LOADING_STATE_PARTLY_LOADED);
			return DR_OK;
		}

		DRReturn TextureSetPixelTask::run()
		{
			model::Texture* m = mViewTexture->getTextureModel();
			if (m->loadFromMemory(mData)) {
				DR_SAVE_DELETE_ARRAY(mData);
				LOG_ERROR("error setting data", DR_ERROR);
			}
			DR_SAVE_DELETE_ARRAY(mData);
			if(!mHasParent)
				mViewTexture->setLoadingState(LOADING_STATE_PARTLY_LOADED);
			if (mFinishCommand) mFinishCommand->taskFinished(this);
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
			TextureLoadingTask* tl = new TextureLoadingTask(this, controller::TextureManager::getInstance()->getTextureCPUScheduler());
#ifdef _UNI_LIB_DEBUG
			tl->setName(mTextureName.data());
#endif
			controller::TaskPtr task(tl);
			task->scheduleTask(task);
			//((controller::CPUTask*)(task.getResourcePtrHolder()->mResource))->start(task);
		}

		void Texture::saveIntoFile(const char* filename)
		{
			controller::TaskPtr task(new TextureSavingTask(this, controller::TextureManager::getInstance()->getTextureCPUScheduler(), filename));
			task->scheduleTask(task);
		}

		

		DRReturn Texture::loadFromMemory(u8* data, controller::Command* finishCommand)
		{
			if (!mTextureModel) 
				LOG_ERROR("texture model not created, size not known!", DR_ERROR);
			controller::TaskPtr task(new TextureSetPixelTask(this, controller::TextureManager::getInstance()->getTextureCPUScheduler(), data));
			if (finishCommand) task->setFinishCommand(finishCommand);
			task->scheduleTask(task);
			
			return DR_OK;
		}
		DRReturn Texture::saveIntoFile(const char* filename, u8* data)
		{
			if (!mTextureModel)
				LOG_ERROR("texture model not created, size not known!", DR_ERROR);
			controller::CPUSheduler* textureScheduler = controller::TextureManager::getInstance()->getTextureCPUScheduler();
			TextureSetPixelTask* setPixel = new TextureSetPixelTask(this, textureScheduler, data);
			setPixel->hasParent();
			controller::TaskPtr setPixelTask(setPixel);
			TextureSavingTask* savingTask = new TextureSavingTask(this, textureScheduler, filename, 1);
			controller::TaskPtr savingTaskPtr(savingTask);
			savingTaskPtr->setParentTaskPtrInArray(setPixelTask, 0);
			mTextureName = filename;
#ifdef _UNI_LIB_DEBUG
			setPixel->setName(filename);
			savingTask->setName(filename);
#endif
			savingTaskPtr->scheduleTask(savingTaskPtr);
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