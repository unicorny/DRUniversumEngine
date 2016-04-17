#include "generator/RenderToTexture.h"
#include "view/Geometrie.h"
#include "view/Material.h"
#include "view/FrameBuffer.h"
#include "view/Texture.h"
#include "controller/BaseGeometrieManager.h"
#include "controller/BindToRenderer.h"
#include "model/UniformSet.h"
#include "model/ShaderProgram.h"

namespace UniLib {
	namespace generator {
		RenderToTexture::RenderToTexture(view::TexturePtr texture)
		 : mTexture(texture) 
		{
			mFrameBuffer = g_RenderBinder->newFrameBuffer(texture);
		}
		RenderToTexture::RenderToTexture()
		{
			mFrameBuffer = g_RenderBinder->newFrameBuffer();
		}

		RenderToTexture::~RenderToTexture()
		{
			DR_SAVE_DELETE(mFrameBuffer);
		}

		bool RenderToTexture::isReady()
		{
			view::Geometrie* geo = controller::BaseGeometrieManager::getInstance()->getGeometrie(controller::BASE_GEOMETRIE_PLANE);
#ifdef _UNI_LIB_DEBUG
			bool geoReady = geo->isReady();
			bool textureReady = mTexture->checkLoadingState() == LOADING_STATE_FULLY_LOADED;
			bool materialReady = mMaterial->checkLoadingState() == LOADING_STATE_FULLY_LOADED;
#endif //_UNI_LIB_DEBUG
			return geo->isReady() && mTexture->checkLoadingState() == LOADING_STATE_FULLY_LOADED && mMaterial->checkLoadingState() == LOADING_STATE_FULLY_LOADED;
		}

		void RenderToTexture::setMaterial(view::Material* mat)
		{
			mMaterial = mat;
			model::UniformSet* uniforms = mMaterial->getUniformSet();
			if (!uniforms) {
				uniforms = g_RenderBinder->newUniformSet();
				mMaterial->setUniformSet(uniforms);
			}

			DRMatrix projection = DRMatrix::ortho_projection(0.0f, 1.0f, 0.0f, 1.0f, -1.0, 1.0);
			uniforms->setUniform("proj", projection);
			model::ShaderProgram* shader = mMaterial->getShaderProgram();
			uniforms->addLocationToUniform("proj", shader);
			
		}
		DRReturn RenderToTexture::run()
		{
			// bind shader and texture
			mMaterial->bind();
			if (mFrameBuffer->isReady()) mFrameBuffer->bind();
			else mFrameBuffer->setup();
			//return DR_OK;
			if (render()) LOG_ERROR("error by rendering", DR_ERROR);
			return DR_OK;
		}
		DRReturn RenderToTexture::render()
		{
			controller::BaseGeometrieManager::getInstance()->getGeometrie(controller::BASE_GEOMETRIE_PLANE)->render();

			return DR_OK;
		}
	}
}