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

		LoadingState TextureMaterial::checkLoadingState()
		{
			if (Material::checkLoadingState() == LOADING_STATE_FULLY_LOADED && mTexture.getResourcePtrHolder() && mTexture->checkLoadingState() == LOADING_STATE_FULLY_LOADED) {
				return LOADING_STATE_FULLY_LOADED;
			}
			if (Material::checkLoadingState() == LOADING_STATE_EMPTY && !mTexture.getResourcePtrHolder() || mTexture->checkLoadingState() == LOADING_STATE_EMPTY) {
				return LOADING_STATE_EMPTY;
			}
			return LOADING_STATE_PARTLY_LOADED;
		}
	}
}