#include "generator/RenderToTexture.h"
#include "view/Geometrie.h"
#include "view/Material.h"
#include "controller/BaseGeometrieManager.h"
#include "model/UniformSet.h"

namespace UniLib {
	namespace generator {
		bool RenderToTexture::isReady()
		{
			view::Geometrie* geo = controller::BaseGeometrieManager::getInstance()->getGeometrie(controller::BASE_GEOMETRIE_PLANE);
			return geo->isReady();
		}

		void RenderToTexture::setMaterial(view::Material* mat)
		{
			mMaterial = mat;
			model::UniformSet* uniforms = mMaterial->getUniformSet();
			if (!uniforms) {
				uniforms = new model::UniformSet();
				mMaterial->setUniformSet(uniforms);
			}
			
			DRMatrix projection = DRMatrix::ortho_projection(0.0f, 1.0f, 0.0f, 1.0f, -1.0, 1.0);
			uniforms->setUniform("proj", projection);
			
		}
	}
}