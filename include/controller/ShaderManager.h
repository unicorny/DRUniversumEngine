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

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_SHADER_MANAGER__
#define __DR_UNIVERSUM_LIB_CONTROLLER_SHADER_MANAGER__

#include "model/ShaderProgram.h"

namespace UniLib {
/*	namespace model {
		struct ShaderProgramParameter;
		class Shader;
		class ShaderProgram;
		enum ShaderType;
		typedef DRResourcePtr<ShaderProgram> ShaderProgramPtr;
		typedef DRResourcePtr<Shader> ShaderPtr;
	}*/
	namespace controller {
		class UNIVERSUM_LIB_API ShaderManager
		{
		public:
			

			// Singleton-Daten
			/*static ShaderManager&		Instance();
			inline static ShaderManager& getSingleton() {return Instance();};
			inline static ShaderManager* getSingletonPtr() {return &Instance();};
			inline static ShaderManager* getInstance() {return &Instance();};
			static bool	isInitialized()	{return Instance().mInitalized;};
			*/
			static ShaderManager* const getInstance();
			__inline__ static bool	isInitialized()	{return getInstance()->mInitalized;};

			DRReturn init();

			void exit();

			//! lädt oder return instance auf Textur
			model::ShaderProgramPtr getShaderProgram(const char* vertexShader, const char* fragmentShader);
			model::ShaderProgramPtr getShaderProgram(model::ShaderProgramParameter* shaderParameter);       

			model::ShaderPtr getShader(const char* shaderName, model::ShaderType shaderType);
		protected:
			ShaderManager();
			ShaderManager(const ShaderManager&);
			virtual ~ShaderManager() {if(mInitalized) exit();};

			DHASH makeShaderHash(const char* vertexShader, const char* fragmentShader);    


			//DRHashList mShaderEntrys;
			std::map<DHASH, model::ShaderProgramPtr>               mShaderProgramEntrys;
			typedef std::pair<DHASH, model::ShaderProgramPtr>      SHADER_PROGRAM_ENTRY;
			std::map<DHASH, model::ShaderPtr>                      mShaderEntrys;
			typedef std::pair<DHASH, model::ShaderPtr>             SHADER_ENTRY;
			bool                                            mInitalized;
#ifdef _UNI_LIB_DEBUG
			std::map<DHASH, model::ShaderProgramParameter>			mHashCollisionCheckMap;
			typedef std::pair<DHASH, model::ShaderProgramParameter> HASH_SHADER_ENTRY;
#endif
		};

	}
}

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_SHADER_MANAGER__