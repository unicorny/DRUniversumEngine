#include "view/MultiTextureMaterial.h"
#include "view/Texture.h"
#include "controller/TextureManager.h"

namespace UniLib {
	namespace view {
		MultiTextureMaterial::MultiTextureMaterial(size_t countTextures)
			: mTextures(new TexturePtr[countTextures]), mTextureCount(countTextures)
		{
		}

		MultiTextureMaterial::~MultiTextureMaterial()
		{
			DR_SAVE_DELETE_ARRAY(mTextures);
		}

		void MultiTextureMaterial::usingTexture(const char* filename, size_t index)
		{
			assert(index < mTextureCount);
			controller::TextureManager* t = controller::TextureManager::getInstance();
			mTextures[index] = t->getTexture(filename);
		}
	}
}