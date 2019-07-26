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
			if (mShaderProgram.getResourcePtrHolder() && mShaderProgram->checkLoadingState() == LOADING_STATE_FULLY_LOADED && mUniformsSet) {
				return LOADING_STATE_FULLY_LOADED;
			}
			if ( (!mShaderProgram.getResourcePtrHolder() && mUniformsSet) ||
				  (!mUniformsSet && mShaderProgram.getResourcePtrHolder()) ) {
				return LOADING_STATE_PARTLY_LOADED;
			}

			return LOADING_STATE_EMPTY;
		}
		
	}
}