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


#ifndef __DR_UNIVERSUM_LIB_VIEW_TEXTURE_H
#define __DR_UNIVERSUM_LIB_VIEW_TEXTURE_H

#include "lib/MultithreadResource.h"
#include "controller/CPUTask.h"
#include "controller/TextureManager.h"
#include "model/Texture.h"

namespace UniLib {


	namespace view {
		class Texture;
		class TextureTask : public controller::CPUTask 
		{
		public:
			TextureTask(view::Texture* texView, controller::CPUSheduler* scheduler)
				: CPUTask(scheduler), mViewTexture(texView) {}
			TextureTask(view::Texture* texView, controller::CPUSheduler* scheduler, u8 dependencyCount)
				: CPUTask(scheduler, dependencyCount), mViewTexture(texView) {}

			virtual const char* getResourceType() const { return "TextureTask"; };
			__inline__ view::Texture* getViewTexture() { return mViewTexture; }

		protected:
			view::Texture* mViewTexture;


		};
		class TextureLoadingTask : public TextureTask
		{
		public:
			TextureLoadingTask(view::Texture* texView, controller::CPUSheduler* scheduler)
				: TextureTask(texView, scheduler) {}
			//! \brief called from task scheduler, maybe from another thread
			virtual DRReturn run();
			virtual const char* getResourceType() const { return "TextureLoadingTask"; };
		};

		class TextureSetPixelTask : public TextureTask
		{
		public:
			TextureSetPixelTask(view::Texture* texView, controller::CPUSheduler* scheduler, u8* data)
				: TextureTask(texView, scheduler), mData(data), mHasParent(false){}

			virtual DRReturn run();
			virtual bool isTaskFinished() { return mData == NULL; }
			__inline__ void hasParent() { mHasParent = true; }
			virtual const char* getResourceType() const { return "TextureSetPixelTask"; };
		protected:
			u8* mData;
			bool mHasParent;
		};

		class TextureGetPixelTask : public TextureTask
		{
		public:
			TextureGetPixelTask(view::Texture* texView, controller::CPUSheduler* scheduler)
				: TextureTask(texView, scheduler) {}
			virtual DRReturn run();
			virtual const char* getResourceType() const { return "TextureGetPixelTask"; };
		};
		class TextureSavingTask : public TextureTask
		{
		public:
			TextureSavingTask(view::Texture* texView, controller::CPUSheduler* scheduler, const char* filename)
				: TextureTask(texView, scheduler), mFilename(filename), mHasParent(false){}
			TextureSavingTask(view::Texture* texView, controller::CPUSheduler* scheduler, const char* filename, u8 dependenceTaskCount)
				: TextureTask(texView, scheduler, dependenceTaskCount), mFilename(filename), mHasParent(true) {}
			virtual DRReturn run();
			virtual const char* getResourceType() const { return "TextureSavingTask"; };
		protected:
			DRString mFilename;
			bool mHasParent;
		};

		class UNIVERSUM_LIB_API Texture : public lib::MultithreadResource
		{
			friend TextureLoadingTask;
			friend TextureSavingTask;
			friend TextureSetPixelTask;
			//friend TextureGetPixelTask;
		public:
			Texture(DHASH id, const char* textureName = NULL);
			Texture(DRVector2i size, GLenum format);
			virtual ~Texture();

			// tasks
			//! \brief load image data from file into memory
			//! \param filename name of file, using FileManager to find file
			void loadFromFile();
			void saveIntoFile(const char* filename);
			//! \brief load image from memory, release data in texture manager thread			
			DRReturn loadFromMemory(u8* data, controller::Command* finishCommand = NULL);
			DRReturn saveIntoFile(const char* filename, u8* data);

			// virtuals, contains renderer specific code
			virtual void uploadToGPU() = 0;
			virtual void downloadFromGPU() = 0;
			virtual void bind() = 0;
			virtual GLuint getTextureId() = 0;
			// virtuals from DRIResource
			virtual const char* getResourceType() const { return "Texture"; }
			virtual bool less_than(DRIResource& b) const {
				return mId < static_cast<Texture&>(b).mId;
			}

			// access parameters
			__inline__ DHASH getId() { return mId; }
			__inline__ std::string getFilename() { return mTextureName; }
			__inline__ Uint32 getTimeout() { return mTimeout; }
			__inline__ void updateTimeout(Uint32 passingTime) { mTimeout -= passingTime; }
			__inline__ void setTimeout(Uint32 timeout) { mTimeout = timeout; }

			__inline__ DRVector2i getTextureSize() { return mTextureModel->getSize(); }

		protected:
			virtual void setLoadingState(LoadingState state);
			__inline__ model::Texture* getTextureModel() { return mTextureModel; }

			model::Texture* mTextureModel;
			std::string mTextureName;

			DHASH mId;
			Uint32 mTimeout;
		};
		
	}
}


#endif //__DR_UNIVERSUM_LIB_VIEW_TEXTURE_H

