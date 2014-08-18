/*/*************************************************************************
 *                                                                         *
 * EngineDLL, Engine for my programs, using SDL and OpenGL		   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/


/**
 * @Author Dario Rekowski
 * 
 * @Date 18.08.12
 * 
 * @Desc Class for easy rsa encryption
 */
 
#ifndef __DR_UNIVERSUM_LIB_RSA__
#define __DR_UNIVERSUM_LIB_RSA__

namespace UniLib {
	namespace lib {
		class UNIVERSUM_LIB_API RSA 
		{
		public:
			RSA();
			~RSA();

			std::string crypt(std::string input, std::string key);

			DRReturn generateKeyPair(std::string& pubKey, std::string& privateKey);


		protected:
			std::string copyStringToOutput(std::string in);
		};
	};
};

#endif //__DR_UNIVERSUM_LIB_RSA__