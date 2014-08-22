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
			~Crypto();

			std::string crypt(std::string input, OperationType type);

			// client keys
			DRReturn generateClientKeys(int validatingLevel = 3);
			std::string getClientPublicKey();
			std::string getClientPrivateKey();

			// server keys
			DRReturn setServerPublicKey(std::string pbKey, int validationLevel = 3);
			std::string getServerPublicKey();

		protected:
			// keys of application client
			CryptoPP::RSA::PublicKey mClientPublicKey;
			CryptoPP::RSA::PrivateKey mClientPrivateKey;
			//std::string mPublicKey;
			//std::string mPrivateKey;

			// public key of server
			CryptoPP::RSA::PublicKey mServerPublic;

			// helper
			std::string queueToBase64String(CryptoPP::BufferedTransformation &bt);
			// return pointer, must be freed
			CryptoPP::BufferedTransformation* base64StringToQueue(std::string base64);

			std::string removePEMHeader(std::string input);

		};
	};
};

#endif //__DR_UNIVERSUM_LIB_CRYPTO__