#include "controller/BaseGeometrieManager.h"
#include "model/geometrie/BaseGeometrie.h"
#include "model/geometrie/Plane.h"
#include "view/Geometrie.h"
#include "controller/GPUScheduler.h"
#include "controller/BindToRenderer.h"
namespace UniLib {
	namespace controller {

		// TASKS 
		DRReturn BaseGeometrieLoadingCPUTask::run()
		{
			model::geometrie::BaseGeometrie* geo = NULL;
			switch (mParent->mType) {
			case BASE_GEOMETRIE_PLANE: geo = new model::geometrie::Plane(model::geometrie::GEOMETRIE_VERTICES);
			}
			if (!geo) {
				EngineLog.writeToLog("type: %d", mParent->mType);
				LOG_ERROR("hasn't found geo base type implementation", DR_ERROR);
			}
			BaseGeometrieManager::getInstance()->getGeometriePointer(mParent->mType)->setBaseGeometrie(geo);
			mParent->ready();
			return DR_OK;
		}

		void BaseGeometrieLoadingGPUTask::ready()
		{
			TaskPtr task(this);
			GPUScheduler::getInstance()->addGPUTask(task, GPU_TASK_LOAD);
		}
		DRReturn BaseGeometrieLoadingGPUTask::run()
		{
			return BaseGeometrieManager::getInstance()->getGeometriePointer(mType)->uploadToGPU();
		}

		void BaseGeometrieLoadingGPUTask::scheduleTask(TaskPtr own)
		{
			own.getResourcePtrHolder()->addRef();
#ifdef _UNI_LIB_DEBUG
			mChildCPUTask->setName(getName());
#endif
			mChildCPUTask->scheduleTask(mChildCPUTask);
		}
		
		//**********************************************************************************

		BaseGeometrieManager* BaseGeometrieManager::getInstance()
		{
			static BaseGeometrieManager theOne;
			return &theOne;
		}

		BaseGeometrieManager::BaseGeometrieManager()
			: mCPUScheduler(NULL), mInitalized(false)
		{
			memset(mGeometrieArray, 0, sizeof(view::Geometrie*)*BASE_GEOMETRIE_TYPE_MAX);
		}
		BaseGeometrieManager::~BaseGeometrieManager()
		{
			exit();
		}
		
		void BaseGeometrieManager::exit()
		{
			for (int i = 0; i < BASE_GEOMETRIE_TYPE_MAX; i++) {
				DR_SAVE_DELETE(mGeometrieArray[i]);
			}
			mInitalized = false;
		}

		DRReturn BaseGeometrieManager::init(CPUSheduler* cpuScheduler/* = NULL*/)
		{
			mCPUScheduler = cpuScheduler;
			mInitalized = true;
			return DR_OK;
		}
		view::Geometrie* BaseGeometrieManager::getGeometrie(BaseGeometrieType type)
		{
			assert(type < BASE_GEOMETRIE_TYPE_MAX);
			assert(mInitalized == true);
			
			if (!mGeometrieArray[type]) {
				mMutex.lock();
				mGeometrieArray[type] = g_RenderBinder->newGeometrie(NULL);
				mMutex.unlock();
				TaskPtr task(new BaseGeometrieLoadingGPUTask(mCPUScheduler, type));
#ifdef _UNI_LIB_DEBUG
				if (type == BASE_GEOMETRIE_PLANE)
					task->setName("Plane");
#endif
				task->scheduleTask(task);
			}

			return mGeometrieArray[type];
		}

	}
}