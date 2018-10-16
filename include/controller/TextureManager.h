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

/*!
 *
 * \author: Dario Rekowski 
 *
 * \date: 06.03.2016
 * 
 * \desc: Singleton Texture Manager for efficiency texture managing
 *        loading and saving by using the best suited scheduler
 *		  stores empty texture buffer for reusing
 */

#ifndef __DR_UNIVERSUM_LIN_CONTROLLER_TEXTURE_MANAGER_H
#define __DR_UNIVERSUM_LIN_CONTROLLER_TEXTURE_MANAGER_H

#include "lib/Singleton.h"
#include "lib/Thread.h"

namespace UniLib {
	namespace lib {
		class MultithreadContainer;
		class Timer;
	}
	namespace view {
		class Texture;
		typedef DRResourcePtr<Texture> TexturePtr;
	}
	namespace controller {
		class CPUSheduler;
		class UNIVERSUM_LIB_API TextureManager : public lib::Singleton
		{
		public:
			static TextureManager* const getInstance();
			__inline__ static bool	isInitialized() { return getInstance()->mInitalized; };

			//! \brief init 
			//! \param defaultCPUSheduler CPUSheduler on which texture loading and saving take place
			//! \param updateTimer timer on which to attach for updating texture storage
			//! \param rerunDelay how much ms passes between update calls
			DRReturn init(lib::Timer* updateTimer, Uint32 rerunDelay = 10000);
			void exit();

			view::TexturePtr getTexture(const char* filename);
			view::TexturePtr getEmptyTexture(DRVector2i size, GLenum format);
			void giveBackEmptyTexture(view::TexturePtr tex);

			// access
			__inline__ void setTimeoutForNotLongerUsedTextures(Uint32 timeout) { mTimeToLetEmptyTexturesInStorage = timeout; }
			__inline__ CPUSheduler* getTextureCPUScheduler() { return mTextureSheduler; }

		protected:
			TextureManager();
			TextureManager(const TextureManager&);
			virtual ~TextureManager() { if (mInitalized) exit(); };


			// member variables
			bool mInitalized;
			CPUSheduler* mTextureSheduler; 
			

			// for updateing
			class UpdateThread : public lib::TimingThread
			{
			public:
				UpdateThread(Uint32 rerunDelay, lib::Timer* timerOnWhichToAttach)
					: lib::TimingThread("TextureManager::Update", rerunDelay, timerOnWhichToAttach, "TexUpd") {}
				virtual int ThreadFunction();
			};

			void update();
			UpdateThread* mUpdateThread;
			Uint32		  mTimeToLetEmptyTexturesInStorage;

			// store existing textures
			typedef std::pair<DHASH, view::TexturePtr> TextureEntry;
			typedef std::map<DHASH, view::TexturePtr> TextureMap;
			typedef std::multimap<DHASH, view::TexturePtr> TextureMultiMap;
			TextureMap mStoredTextures;
			TextureMultiMap mEmptyTextures;
		};
	}
}

#endif