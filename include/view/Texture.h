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

namespace UniLib {

	namespace model {
		class Texture;
	}
	namespace view {
		class Texture;
		class TextureLoadingTask : public controller::CPUTask
		{
		public:
			TextureLoadingTask(view::Texture* texView, controller::CPUSheduler* scheduler)
				: CPUTask(scheduler), mViewTexture(texView) {}

			//! \brief called from task scheduler, maybe from another thread
			virtual DRReturn run();
		
		protected:
			view::Texture* mViewTexture;
		};

		class UNIVERSUM_LIB_API Texture : public lib::MultithreadResource
		{
			friend TextureLoadingTask;
		public:
			Texture(DHASH id, const char* textureName = NULL);
			Texture(DRVector2i size, GLenum format);
			virtual ~Texture();

			// tasks
			//! \brief load image data from file into memory
			//! \param filename name of file, using FileManager to find file
			void loadFromFile();
			virtual void saveIntoFile(const char* filename) = 0;

			// virtuals, contains renderer specific code
			virtual void uploadToGPU() = 0;
			virtual void downloadFromGPU() = 0;
			virtual void bind() = 0;

			// virtuals from DRIResource
			virtual const char* getResourceType() const { return "Texture"; }
			virtual bool less_than(DRIResource& b) const {
				return mId < static_cast<Texture&>(b).mId;
			}

			// access parameters
			__inline__ DHASH getId() { return mId; }
			__inline__ std::string getFilename() { return mTextureName; }

		protected:
			virtual void setLoadingState(LoadingState state);
			__inline__ model::Texture* getTextureModel() { return mTextureModel; }

			model::Texture* mTextureModel;
			std::string mTextureName;

			DHASH mId;
		};
		
	}
}


#endif //__DR_UNIVERSUM_LIB_VIEW_TEXTURE_H

