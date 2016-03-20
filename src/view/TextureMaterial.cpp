#include "view/TextureMaterial.h"
#include "view/Texture.h"
#include "controller/TextureManager.h"

namespace UniLib {
	namespace view {
		TextureMaterial::TextureMaterial()
		{

		}

		TextureMaterial::~TextureMaterial()
		{

		}

		void TextureMaterial::usingTexture(const char* filename)
		{
			controller::TextureManager* t = controller::TextureManager::getInstance();
			mTexture = t->getTexture(filename);
		}
	}
}