#include "controller/FrameBufferRenderCall.h"
#include "generator/RenderToTexture.h"

namespace UniLib {
	namespace controller {
		FrameBufferRenderCall::FrameBufferRenderCall()
		{

		}

		FrameBufferRenderCall::~FrameBufferRenderCall()
		{
			lock();
			while (!mFastRenderToTextureTasks.empty()) {
				generator::RenderToTexture* r = mFastRenderToTextureTasks.front();
				DR_SAVE_DELETE(r);
				mFastRenderToTextureTasks.pop_front();
			}
			unlock();
		}

		DRReturn FrameBufferRenderCall::render(float timeSinceLastFrame)
		{
			lock();
			while (!mFastRenderToTextureTasks.empty()) {
				generator::RenderToTexture* r = mFastRenderToTextureTasks.front();
				if (!r->isReady()) break;
				if (r->prepareRendering()) {
					LOG_WARNING("error by preparing one RenderToTexture Task");
				}
				else if(r->render()) {
					LOG_WARNING("error by rendering one RenderToTexture Task");
				}
				DR_SAVE_DELETE(r);
				mFastRenderToTextureTasks.pop_front();
			}
			unlock();
			return DR_OK;
		}
		// if render return not DR_OK, Call will be removed from List and kicked will be called
		void FrameBufferRenderCall::kicked()
		{
			LOG_ERROR_VOID("I was kicked");
		}
		// will be called if render call need to much time
		// \param percent used up percent time of render main loop
		void FrameBufferRenderCall::youNeedToLong(float percent)
		{
			printf("[FrameBufferRenderCall] used up to much GPU time: %f\n", percent);
		}

		DRReturn FrameBufferRenderCall::addRenderToTextureTask(generator::RenderToTexture* task, bool fastTask/* = true*/)
		{
			assert(task != NULL);
			if (!fastTask) LOG_ERROR("slow render to texture task not implented yet!", DR_ERROR);
			lock();
			mFastRenderToTextureTasks.push_back(task);
			unlock();
			return DR_OK;
		}
	}
}