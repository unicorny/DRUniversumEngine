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
				CRYPT_PUBLIC = 1,
				CRYPT_PRIVATE = 2,
				UNCRYPT_PUBLIC = 3,
				UNCRYPT_PRIVATE = 4
			};
			Crypto();
			~Crypto();

			std::string crypt(std::string input, OperationType type);

			DRReturn generateKeys();
			__inline__ std::string getPublicKey() {return mPublicKey;}
			__inline__ void setPublicKey(std::string key) {mPublicKey = key;}
			__inline__ std::string getPrivateKey() {return mPrivateKey;}
			__inline__ void setPrivateKey(std::string key) {mPrivateKey = key;}

			DRReturn setServerPublicKey(std::string pbKey, int validationLevel);

			DRReturn getPublicKeyFromPEM(std::string pemPublickKey, CryptoPP::RSA::PublicKey& pbKey);

		protected:
			// keys of application client
			std::string mPublicKey;
			std::string mPrivateKey;

			// public key of server
			CryptoPP::RSA::PublicKey mServerPublic;

			DRReturn loadPublicKeyFromString(std::string );
		};
	};
};

#endif //__DR_UNIVERSUM_LIB_CRYPTO__