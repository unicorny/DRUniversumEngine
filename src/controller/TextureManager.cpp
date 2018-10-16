#include "controller/TextureManager.h"
#include "controller/BindToRenderer.h"
#include "controller/CPUSheduler.h"

#include "model/Texture.h"
#include "view/Texture.h"

#include "lib/MultithreadContainer.h"
#include "lib/Timer.h"

namespace UniLib {
	namespace controller {

		TextureManager::TextureManager()
			: mInitalized(false), mTextureSheduler(NULL), mUpdateThread(NULL)
		{

		}

		TextureManager::TextureManager(const TextureManager&)
			: mInitalized(false), mTextureSheduler(NULL)
		{

		}

		TextureManager* const TextureManager::getInstance()
		{
			static TextureManager TheOneAndOnly;
			return &TheOneAndOnly;
		}

		DRReturn TextureManager::init(lib::Timer* updateTimer, Uint32 rerunDelay/* = 10000*/)
		{
			if (g_HarddiskScheduler && g_HarddiskScheduler->getThreadCount() == 1)
				mTextureSheduler = g_HarddiskScheduler;
			else 
				mTextureSheduler = new CPUSheduler(1, "texture");
			mUpdateThread = new UpdateThread(rerunDelay, updateTimer);
			// wait 20 seconds before deleting not used textures, maybe some other thread need the memory
			mTimeToLetEmptyTexturesInStorage = 20000;
			mInitalized = true;
			LOG_INFO("TextureManager initalisiert");
			return DR_OK;
		}

		void TextureManager::exit()
		{
			// not necessary, because will be deletet from timer
			//DR_SAVE_DELETE(mUpdateThread);
			if(g_HarddiskScheduler != mTextureSheduler)
				DR_SAVE_DELETE(mTextureSheduler);
			mInitalized = false;
			LOG_INFO("TextureManager beendet");
		}

		view::TexturePtr TextureManager::getTexture(const char* filename)
		{
			assert(g_RenderBinder != NULL);

			DHASH id = DRMakeFilenameHash(filename);
			mUpdateThread->threadLock();
			TextureMap::iterator it = mStoredTextures.find(id);
			if (it != mStoredTextures.end()) {
				mUpdateThread->threadUnlock();
				return it->second;
			}
			mUpdateThread->threadUnlock();
			view::TexturePtr tex = view::TexturePtr(g_RenderBinder->newTexture(id, filename));
			tex->loadFromFile();
			mUpdateThread->threadLock();
			mStoredTextures.insert(TextureEntry(id, tex));
			mUpdateThread->threadUnlock();
			return tex;
		}

		view::TexturePtr TextureManager::getEmptyTexture(DRVector2i size, GLenum format)
		{
			assert(g_RenderBinder != NULL);

			DHASH id = model::Texture::calculateHash(size, format);
			mUpdateThread->threadLock();
			TextureMultiMap::iterator it = mEmptyTextures.find(id);
			if (it != mEmptyTextures.end()) {
				view::TexturePtr tex = it->second;
				mEmptyTextures.erase(it);
				mUpdateThread->threadUnlock();
				return tex;
			}
			mUpdateThread->threadUnlock();
			view::TexturePtr tex = view::TexturePtr(g_RenderBinder->newTexture(size, format));
			tex->uploadToGPU();
			//mUpdateThread->threadLock();
			//mEmptyTextures.insert(TextureEntry(id, tex));
			//mUpdateThread->threadUnlock();
			return tex;
		}

		void TextureManager::giveBackEmptyTexture(view::TexturePtr tex)
		{
			DHASH id = tex->getId();
			mUpdateThread->threadLock();
			mEmptyTextures.insert(TextureEntry(id, tex));
			mUpdateThread->threadUnlock();
		}

		// Timer
		int TextureManager::UpdateThread::ThreadFunction()
		{
			TextureManager::getInstance()->update();
			return 0;
		}

		void TextureManager::update()
		{
			if (!mInitalized) return;
			static Uint32 lastTicks = 0;
			if (!lastTicks) {
				lastTicks = SDL_GetTicks();
				return;
			}
			Uint32 passingTime = SDL_GetTicks() - lastTicks;
			// update and check multimap timeout
			for (TextureMultiMap::iterator it = mEmptyTextures.begin(); it != mEmptyTextures.end(); it++) {
				view::TexturePtr tex = it->second;
				tex->updateTimeout(passingTime);
				// enough time has passed, we remove texture storage from memory
				if (tex->getTimeout() <= 0) {
					it = mEmptyTextures.erase(it);
				}
				
			}
			// check map and removing not longer used textures
			for (TextureMap::iterator it = mStoredTextures.begin(); it != mStoredTextures.end(); it++) {
				view::TexturePtr tex = it->second;
				if (tex.getResourcePtrHolder()->getRefCount() == 1) {
					tex->setTimeout(mTimeToLetEmptyTexturesInStorage);
					mEmptyTextures.insert(TextureEntry(it->first, it->second));
					it = mStoredTextures.erase(it);
				}
			}
			lastTicks = SDL_GetTicks();

		}

	}
}