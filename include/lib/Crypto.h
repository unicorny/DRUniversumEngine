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
 
#ifndef __DR_UNIVERSUM_LIB_CRYPTO__
#define __DR_UNIVERSUM_LIB_CRYPTO__


namespace UniLib {
	namespace lib {
		class UNIVERSUM_LIB_API Crypto 
		{
		public:
			enum OperationType {
				CRYPT_WITH_CLIENT_PUBLIC = 1,
				CRYPT_WITH_CLIENT_PRIVATE = 2,
				UNCRYPT_WITH_SERVER_PUBLIC = 3,
				CRYPT_WITH_SERVER_PUBLIC = 4,
				UNCRYPT_WITH_CLIENT_PRIVATE = 5,
				UNCRYPT_WITH_CLIENT_PUBLIC = 6
			};
			Crypto();
			virtual ~Crypto();

			virtual std::string crypt(std::string input, OperationType type) = 0;
			virtual std::string sign(std::string input) = 0;
			virtual bool checkSign(std::string input, std::string signature) = 0;

			// client keys
			virtual DRReturn generateClientKeys() = 0;
			virtual std::string getClientPublicKey() = 0;
			virtual std::string getClientPrivateKey() = 0;

			// server keys
			virtual DRReturn setServerPublicKey(std::string pbKey, int validationLevel = 3) = 0;
			virtual std::string getServerPublicKey() = 0;					

			// static strings
			static std::string getOperationTypeString(OperationType type);

		};
	};
};

#endif //__DR_UNIVERSUM_LIB_CRYPTO__