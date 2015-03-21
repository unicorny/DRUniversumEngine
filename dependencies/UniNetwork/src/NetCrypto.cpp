#include "NetCrypto.h"

#include "Poco/Base64Decoder.h"
#include "Poco/HexBinaryEncoder.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherFactory.h"

#include <sstream>

NetCrypto::NetCrypto()
	: Loggable("crypto.log", "NetCrypto"), mClientKey(NULL), mServerKey(NULL), mServerPublicKeyValid(false)
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
	case UNCRYPT_WITH_CLIENT_PRIVATE:
		pCipher = Poco::Crypto::CipherFactory::defaultFactory().createCipher(*mClientKey, RSA_PADDING_PKCS1);
		crypt = false;
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

std::string NetCrypto::sign(std::string input)
{
	try {
		Poco::Crypto::RSADigestEngine eng(*mClientKey, Poco::Crypto::RSADigestEngine::DIGEST_SHA1);
		eng.update(input.c_str(), static_cast<unsigned>(input.length()));
		const Poco::DigestEngine::Digest& sig = eng.signature();
		return Poco::DigestEngine::digestToHex(sig);
	} catch(Poco::Exception what) {
		POCO_LOG_FATAL(std::string("error by sign with input: ") + input + what.displayText());
		return "";
	}
	return "";
}

bool NetCrypto::checkSign(std::string input, std::string signature)
{
	// verify
	try {
		Poco::Crypto::RSADigestEngine eng(*mServerKey);
		//Poco::Crypto::RSADigestEngine eng(*mClientKey);
		eng.update(input.c_str(), static_cast<unsigned>(input.length()));
		//const Poco::DigestEngine::Digest& sig = eng.signature();
		const Poco::DigestEngine::Digest& sig = Poco::Crypto::RSADigestEngine::digestFromHex(signature);
		return eng.verify(sig);
		//std::string hexSignature = Poco::DigestEngine::digestToHex(sig);
		//if(hexSignature.compare(signature) == 0) return true;
		//eng2.verify(signature);

	} catch(Poco::Exception what) {
			POCO_LOG_FATAL(std::string("error by checking signature with input: ") + input +
					std::string(", signature: ") + signature + std::string(", error: ") + what.displayText());
	}
	return false;
}

// client keys
DRReturn NetCrypto::generateClientKeys()
{
	DR_SAVE_DELETE(mClientKey);
	mClientKey = new Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_2048,
										  Poco::Crypto::RSAKey::EXP_LARGE);
	
	std::ostringstream publicKey;
	std::ostringstream privateKey;
	//std::ostringstream hexEncodedKey;
	//Poco::HexBinaryEncoder encode(hexEncodedKey);
	//mClientKey->save(&publicKey, &privateKey, "as723kas78DKLJash8d3");
	//mClientPublicKeyString = publicKey.str();
	//mClientPrivateKeyString = privateKey.str();
	mClientKey->save(&publicKey, &privateKey, "as723kas78DKLJash8d3");
	//encode.close();
	mClientPublicKeyString = publicKey.str();
	mClientPrivateKeyString = privateKey.str();

	return DR_OK;
}
std::string NetCrypto::getClientPublicKey(OutputType outputType/* = STRING*/)
{
	if(outputType == STRING) {
		return mClientPublicKeyString;
	} else {
		return encodeToHex(mClientPublicKeyString);
	}
}
std::string NetCrypto::getClientPrivateKey(OutputType outputType/* = STRING*/)
{
	if(outputType == STRING) {
		return mClientPrivateKeyString;
	} else {
		return encodeToHex(mClientPrivateKeyString);
	}
}

std::string NetCrypto::encodeToHex(std::string input) 
{
	std::ostringstream hexEncoded;
	Poco::HexBinaryEncoder encode(hexEncoded);
	encode << input;
	encode.close();
	return hexEncoded.str();
}

// server keys
DRReturn NetCrypto::setServerPublicKey(const std::string& pbKey, int validationLevel/* = 3*/)
{
	DR_SAVE_DELETE(mServerKey);
	//std::istringstream str(removePEMHeader(pbKey));
	//Poco::Base64Decoder decode(str);
	//Poco::Base64Decoder
	mServerPublicKeyString = pbKey;
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
	mServerPublicKeyValid = true;
	return DR_OK;
}


std::string NetCrypto::getServerPublicKey(OutputType outputType/* = STRING*/)
{
	if(outputType == STRING) {
		return mServerPublicKeyString;
	} else {
		return encodeToHex(mServerPublicKeyString);
	}
}
