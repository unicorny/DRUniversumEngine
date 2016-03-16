#include "view/Material.h"
#include "view/Texture.h"
#include "controller/TextureManager.h"

namespace UniLib {
	namespace view {
		Material::Material() 
		{

		}

		Material::~Material()
		{

		}

		void Material::usingTexture(const char* filename)
		{
			controller::TextureManager* t = controller::TextureManager::getInstance();
			mTexture = t->getTexture(filename);
		}
	}
}