/*/*************************************************************************
*                                                                         *
* UniversumLib, collection of classes for generating and go through a     *
* whole universe. It is for my Gameproject Spacecraft					   *
* Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
* Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
*                                                                         *
* This program is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* any later version.													   *
*																		   *
* This program is distributed in the hope that it will be useful,	       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
* GNU General Public License for more details.							   *
*																		   *
* You should have received a copy of the GNU General Public License	   *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                         *
***************************************************************************/

#ifndef __UNIVERSUM_LIB_CONTROLLER_BASE_GEOMETRIE_MANAGER_H
#define __UNIVERSUM_LIB_CONTROLLER_BASE_GEOMETRIE_MANAGER_H
/*!
* \author Dario Rekowski
*
* \date 20.03.16
*
* \desc Contains Base Geometries
*
*/

#include "lib/Singleton.h"
#include "lib/MultithreadContainer.h"
#include "controller/GPUTask.h"
#include "controller/CPUTask.h"
namespace UniLib {
	namespace view {
		class Geometrie;
	}
	namespace controller {
		
		class GPUScheduler;
		class CPUSheduler;
		enum BaseGeometrieType {
			BASE_GEOMETRIE_PLANE,
			BASE_GEOMETRIE_TYPE_MAX
			
		};
		class BaseGeometrieLoadingGPUTask;
		class BaseGeometrieLoadingCPUTask : public CPUTask {
		public:
			BaseGeometrieLoadingCPUTask(CPUSheduler* scheduler, BaseGeometrieLoadingGPUTask* parent)
				: CPUTask(scheduler), mParent(parent) {}
			virtual DRReturn run();
			virtual const char* getResourceType() const { return "BaseGeometrieLoadingCPUTask"; };
		protected:
			BaseGeometrieLoadingGPUTask* mParent;
		};
		class BaseGeometrieLoadingGPUTask : public GPUTask
		{
			friend BaseGeometrieLoadingCPUTask;
		public:
			BaseGeometrieLoadingGPUTask(CPUSheduler* scheduler, BaseGeometrieType geoType)
				: mType(geoType), mChildCPUTask(new BaseGeometrieLoadingCPUTask(scheduler, this)) {}
			virtual const char* getResourceType() const { return "BaseGeometrieLoadingGPUTask"; };
			
			virtual DRReturn run();
			virtual void scheduleTask(TaskPtr own);
		protected:
			void ready();
			BaseGeometrieType mType;
			TaskPtr mChildCPUTask;
		};
		class UNIVERSUM_LIB_API BaseGeometrieManager : public lib::Singleton
		{
			friend BaseGeometrieLoadingCPUTask;
			friend BaseGeometrieLoadingGPUTask;
		public:
			static BaseGeometrieManager* getInstance();

			DRReturn init(CPUSheduler* cpuScheduler = NULL);
			void exit();

			//! \brief get pointer to geometrie, create if necessary
			
			view::Geometrie* getGeometrie(BaseGeometrieType type);
		protected:

			__inline__ view::Geometrie* getGeometriePointer(BaseGeometrieType type) { return mGeometrieArray[type]; }
			BaseGeometrieManager();
			virtual ~BaseGeometrieManager();

			view::Geometrie* mGeometrieArray[BASE_GEOMETRIE_TYPE_MAX];
			CPUSheduler* mCPUScheduler;
			bool mInitalized;
			lib::MultithreadContainer mMutex;
		};
	}
}

#endif __UNIVERSUM_LIB_CONTROLLER_BASE_GEOMETRIE_MANAGER_H