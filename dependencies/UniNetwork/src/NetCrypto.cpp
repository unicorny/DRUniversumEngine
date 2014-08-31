#include "UniNetworkHeader.h"


NetCrypto::NetCrypto()
	: Loggable("crypto.log", "NetCrypto"), mClientKey(NULL), mServerKey(NULL)
{
}

NetCrypto::~NetCrypto()
{
	DR_SAVE_DELETE(mClientKey);
	DR_SAVE_DELETE(mServerKey);

}

std::string NetCrypto::crypt(std::string input, OperationType type)
{
	Poco::Crypto::Cipher::Ptr pCipher;// = Poco::Crypto::CipherFactory::defaultFactory().createCipher(publicKey);
	bool crypt = true;// if set to false, uncrypt
	switch(type) {
	case CRYPT_WITH_SERVER_PUBLIC:
		pCipher = Poco::Crypto::CipherFactory::defaultFactory().createCipher(*mServerKey);
		break;
	default: 
		POCO_LOG_WARNING(std::string("operation type isn't implemented, " + getOperationTypeString(type)));
		return "";
	};
	if(crypt){
		return pCipher->encryptString(input, Poco::Crypto::Cipher::ENC_BASE64);
	} else {
		return pCipher->decryptString(input, Poco::Crypto::Cipher::ENC_BASE64);
	}

	return "";
}

// client keys
DRReturn NetCrypto::generateClientKeys()
{
	return DR_OK;
}
std::string NetCrypto::getClientPublicKey()
{
	return "";
}
std::string NetCrypto::getClientPrivateKey()
{
	return "";
}

// server keys
DRReturn NetCrypto::setServerPublicKey(std::string pbKey, int validationLevel/* = 3*/)
{
	DR_SAVE_DELETE(mServerKey);
	//std::istringstream str(removePEMHeader(pbKey));
	//Poco::Base64Decoder decode(str);
	std::istringstream str(pbKey);
	Poco::Crypto::X509Certificate cert(str);
	try {
		mServerKey = new Poco::Crypto::RSAKey(cert);
	} catch(Poco::Exception what) {
		//what.displayText
		//LOG_ERROR("Poco throw an exception", DR_ERROR);
		//printf(what.displayText().data());
		POCO_LOG_ERROR(what.displayText());
		return DR_ERROR;
	}
	
	return DR_OK;
}


std::string NetCrypto::getServerPublicKey()
{
	return "";
}
