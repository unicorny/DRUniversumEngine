#include "UniversumLib.h"


using namespace CryptoPP;

namespace UniLib {
	namespace lib {
		RSA::RSA()
		{
		}

		RSA::~RSA() 
		{
		}

		DRReturn RSA::generateKeyPair(std::string& pubKey, std::string& privateKey)
		{

			// InvertibleRSAFunction is used directly only because the private key
			// won't actually be used to perform any cryptographic operation;
			// otherwise, an appropriate typedef'ed type from rsa.h would have been used.
			AutoSeededRandomPool rng;
			InvertibleRSAFunction privkey; 
			privkey.Initialize(rng, 1024);
 
			// With the current version of Crypto++, MessageEnd() needs to be called
			// explicitly because Base64Encoder doesn't flush its buffer on destruction.
			std::string privateKeyTemp;
			Base64Encoder privkeysink(new StringSink(privateKeyTemp));
			privkey.DEREncode(privkeysink);
			privkeysink.MessageEnd();
 
			// Suppose we want to store the public key separately,
			// possibly because we will be sending the public key to a third party.
			RSAFunction pubkey(privkey);
 
			std::string publicKeyTemp;
			Base64Encoder pubkeysink(new StringSink(publicKeyTemp));
			pubkey.DEREncode(pubkeysink);
			pubkeysink.MessageEnd();

		
			//pubKey = publicKeyTemp;
			//pubKey = copyStringToOutput(publicKeyTemp);
			//privateKey = copyStringToOutput(privateKeyTemp);
			//privateKey = privateKeyTemp;
			UniLib::EngineLog.writeToLog("create public key: %s, private Key: %s", copyStringToOutput(publicKeyTemp).data(), copyStringToOutput(privateKeyTemp).data());
			return DR_OK;
		}

		std::string RSA::crypt(std::string input, std::string key)
		{
			return "";
		}

		std::string RSA::copyStringToOutput(std::string in)
		{
			char* buffer = new char[in.size()+1];
			memcpy(buffer, in.data(), in.size());
			std::string out = buffer;
			DR_SAVE_DELETE_ARRAY(buffer);
			return out;
		}

	}
}