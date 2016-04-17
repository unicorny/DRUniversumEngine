#include "view/Material.h"
#include "model/ShaderProgram.h"

namespace UniLib {
	namespace view {
		Material::Material() 
			: mUniformsSet(NULL)
		{

		}

		Material::~Material()
		{

		}

		LoadingState Material::checkLoadingState()
		{
			return mShaderProgram->checkLoadingState();
		}
		
	}
}