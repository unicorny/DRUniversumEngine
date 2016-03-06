#include "controller/TextureManager.h"
#include "controller/BindToRenderer.h"

#include "model/Texture.h"
#include "view/Texture.h"

namespace UniLib {
	namespace controller {

		TextureManager::TextureManager()
			: mInitalized(false), mDefaultSheduler(NULL)
		{

		}

		TextureManager::TextureManager(const TextureManager&)
			: mInitalized(false), mDefaultSheduler(NULL)
		{

		}

		TextureManager* const TextureManager::getInstance()
		{
			static TextureManager TheOneAndOnly;
			return &TheOneAndOnly;
		}

		DRReturn TextureManager::init(CPUSheduler* defaultCPUSheduler)
		{
			mDefaultSheduler = defaultCPUSheduler;
			mInitalized = true;
			LOG_INFO("TextureManager initalisiert");
			return DR_OK;
		}

		void TextureManager::exit()
		{
			mInitalized = false;
			LOG_INFO("TextureManager beendet");
		}

		view::TexturePtr TextureManager::getTexture(const char* filename)
		{
			assert(g_RenderBinder != NULL);

			DHASH id = DRMakeFilenameHash(filename);
			TextureMap::iterator it = mStoredTextures.find(id);
			if (it != mStoredTextures.end()) {
				return it->second;
			}
			
			view::TexturePtr tex = view::TexturePtr(g_RenderBinder->newTexture(filename));
		}

		view::TexturePtr TextureManager::getEmptyTexture(DRVector2i size, GLenum format)
		{
			assert(g_RenderBinder != NULL);

			DHASH id = model::Texture::calculateHash(size, format);
			TextureMultiMap::iterator it = mEmptyTextures.find(id);
			if (it != mEmptyTextures.end()) {
				return it->second;
			}
			view::TexturePtr tex = view::TexturePtr(g_RenderBinder->newTexture(size, format));
		}



	}
}