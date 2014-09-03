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
		pCipher = Poco::Crypto::CipherFactory::defaultFactory().createCipher(*mServerKey, RSA_PADDING_PKCS1);
		break;
	default: 
		POCO_LOG_WARNING(std::string("operation type isn't implemented, " + getOperationTypeString(type)));
		return "";
	};
	try {
		if(crypt){
			return pCipher->encryptString(input, Poco::Crypto::Cipher::ENC_BINHEX_NO_LF);
		} else {
			return pCipher->decryptString(input, Poco::Crypto::Cipher::ENC_BINHEX_NO_LF);
		}
	} catch(Poco::IOException exception) {
		POCO_LOG_FATAL(std::string("error by crypt with input: ") + input);
		return "";
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
	//Poco::Base64Decoder
	std::istringstream str(pbKey);
	
	try {
		Poco::Crypto::X509Certificate cert(str);
		mServerKey = new Poco::Crypto::RSAKey(cert);
	} catch(Poco::IOException what) {
		POCO_LOG_FATAL(std::string("io exception: ") + what.displayText() + std::string(", with pbkey: ") + pbKey);
		return DR_ERROR;
	}
	catch(Poco::Exception what) {
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
