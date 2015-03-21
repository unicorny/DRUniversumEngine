/*/*************************************************************************
 *                                                                         *
 * UniNetwork, Library for Netwerk Communication for my					   *
 * Gameproject Spacecraft												   *			 
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

#ifndef __NETWORK_NET_CRYPTO_H__
#define __NETWORK_NET_CRYPTO_H__

#include "lib/Crypto.h"
#include "Loggable.h"

namespace Poco {
    namespace Crypto {
        class RSAKey;
    }
}

class UNI_NETWORK_API NetCrypto : public UniLib::lib::Crypto, Loggable 
{
public:
	NetCrypto();
	virtual ~NetCrypto();

	virtual std::string crypt(std::string input, OperationType type);

	// client keys
	virtual DRReturn generateClientKeys();
	virtual std::string getClientPublicKey(OutputType outputType = STRING);
	virtual std::string getClientPrivateKey(OutputType outputType = STRING);

	//signature
	virtual std::string sign(std::string input);
	virtual bool checkSign(std::string input, std::string signature);

	// server keys
	virtual DRReturn setServerPublicKey(const std::string& pbKey, int validationLevel = 3);
	virtual std::string getServerPublicKey(OutputType outputType = STRING);
	virtual __inline__ bool isServerPublicKeyExist() {return mServerPublicKeyValid;}

protected:
	std::string encodeToHex(std::string input);

	Poco::Crypto::RSAKey*	mClientKey;
	std::string				mClientPublicKeyString;
	std::string				mClientPrivateKeyString;

	Poco::Crypto::RSAKey*	mServerKey;
	std::string             mServerPublicKeyString; 
	bool					mServerPublicKeyValid;
};


#endif //__NETWORK_NET_CRYPTO_H__