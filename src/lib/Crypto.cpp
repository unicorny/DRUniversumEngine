#include "UniversumLib.h"


using namespace CryptoPP;

namespace UniLib {
	namespace lib {
		Crypto::Crypto()
		{
		}

		Crypto::~Crypto() 
		{
		}

		

		DRReturn Crypto::generateClientKeys(int validatingLevel/* = 3*/)
		{
			// Generate keys
			AutoSeededRandomPool rng;

			InvertibleRSAFunction params;
			params.GenerateRandomWithKeySize( rng, 2048 );

			mClientPrivateKey = RSA::PrivateKey( params );
			mClientPublicKey =  RSA::PublicKey( params );
			
			if(!mClientPrivateKey.Validate(rng, validatingLevel))
				LOG_ERROR("Key isn't valid", DR_ERROR);

			return DR_OK;
		}

		std::string Crypto::crypt(std::string input, OperationType type)
		{
			////////////////////////////////////////////////
			// Generate keys
			AutoSeededRandomPool rng;

			InvertibleRSAFunction params;
			params.GenerateRandomWithKeySize( rng, 1536 );

			RSA::PrivateKey privateKey( params );
			RSA::PublicKey publicKey( params );

			std::string plain="RSA Encryption", cipher, recovered;

			////////////////////////////////////////////////
			// Encryption
			RSAES_OAEP_SHA_Encryptor e( publicKey );

			StringSource ss1( plain, true,
				new PK_EncryptorFilter( rng, e,
					new StringSink( cipher )
				) // PK_EncryptorFilter
			 ); // StringSource

			////////////////////////////////////////////////
			// Decryption
			RSAES_OAEP_SHA_Decryptor d( privateKey );

			StringSource ss2( cipher, true,
				new PK_DecryptorFilter( rng, d,
					new StringSink( recovered )
				) // PK_DecryptorFilter
			 ); // StringSource

			assert( plain == recovered );
			return "";
		}

		// client keys		
		std::string Crypto::getClientPublicKey()
		{
			ByteQueue queue;
			mClientPublicKey.Save(queue);
			return queueToBase64String(queue);
		}
		std::string Crypto::getClientPrivateKey()
		{
			ByteQueue queue;
			mClientPrivateKey.Save(queue);
			return queueToBase64String(queue);
		}

		// server keys
		DRReturn Crypto::setServerPublicKey(std::string pbKey, int validationLevel /* = 3*/)
		{
			AutoSeededRandomPool rng;
			ByteQueue queue;
			base64StringToQueue(removePEMHeader(pbKey), queue);
			try {
				mServerPublic.Load(queue);
			}
			catch(CryptoPP::BERDecodeErr err) 
			{
				EngineLog.writeToLog("Crypto Error: %s", err.what());
				LOG_ERROR("error by loading key", DR_ERROR);
			}
			FileSink fs("serverPublic.pem");
			Base64Encoder encoder;
			ByteQueue out;

			// encode
			encoder.Attach(new Redirector(out));
			mServerPublic.Save(encoder);
			encoder.MessageEnd();

			// copy to file
			out.CopyTo(fs);
			fs.MessageEnd();
			if(mServerPublic.Validate(rng, validationLevel))
				LOG_ERROR("key isn't valid", DR_ERROR);
			return DR_OK;
		}

		DRReturn Crypto::setServerPublicKey(std::string e, std::string n, int validationLevel /* = 3*/)
		{
			AutoSeededRandomPool rng;

			ByteQueue eQ;
			ByteQueue nQ;
			char test[] = "Mg==";
			base64StringToQueue(e, eQ);
			//base64StringToQueue(n, nQ);
			try {
				//mServerPublic.SetModulus(Integer(eQ));
				//Integer i(eQ);
				std::string stest;
				StringSink ss(stest);
				eQ.TransferTo(ss);
				ss.MessageEnd();
				UniLib::EngineLog.writeToLog("long: %s",  stest.data());
				//mServerPublic.SetPublicExponent(Integer(nQ));
			} catch(CryptoPP::Exception err) {
				UniLib::EngineLog.writeToLog("exception: %s", err.what());
				LOG_ERROR("error by encode to key", DR_ERROR);
			}

			if(mServerPublic.Validate(rng, validationLevel)) 
				LOG_ERROR("key isn't valid", DR_ERROR);
			
			return DR_OK;
		}

		std::string Crypto::getServerPublicKey() 
		{
			ByteQueue queue;
			mServerPublic.Save(queue);
			return queueToBase64String(queue);
		}

		std::string Crypto::queueToBase64String(BufferedTransformation &bt)
		{
			Base64Encoder encoder;
			ByteQueue out;
			std::string stringOut;
			StringSink ss(stringOut);

			// encode
			encoder.Attach(new Redirector(out));
			bt.CopyTo(encoder);
			encoder.MessageEnd();

			// copy to string
			out.CopyTo(ss);
			ss.MessageEnd();

			return stringOut;
		}

		DRReturn Crypto::base64StringToQueue(std::string base64, CryptoPP::BufferedTransformation &bt)
		{
			try {
				StringSource ss(base64, true, new Base64Decoder(new Redirector(bt)));
			} catch(CryptoPP::Exception err) {
				UniLib::EngineLog.writeToLog("exception: %s", err.what());
				LOG_ERROR("error by base64 decoding to key", DR_ERROR);
			}
			return DR_OK;
			/*Base64Decoder decoder;
			//FileSource loadFile("rsa-roundtrip.key", true);
			StringSource ss(base64, true);

			try {
				ss.TransferTo(decoder);
				decoder.MessageEnd();
				decoder.TransferTo(bt);
				bt.MessageEnd();
			} catch(CryptoPP::BERDecodeErr err) {
				UniLib::EngineLog.writeToLog("error by decode string: %s", err.what());
				LOG_ERROR("error", DR_ERROR);
			}
			return DR_OK;
			//*/
		}

		std::string Crypto::removePEMHeader(std::string input)
		{
			static std::string HEADER = "-----BEGIN PUBLIC KEY-----\n";
			static std::string FOOTER = "\n-----END";
    
			size_t pos1, pos2;
			pos1 = input.find(HEADER);
			if(pos1 == std::string::npos)
				pos1 = 0;
    
			pos2 = input.find(FOOTER, pos1+1);
			//if(pos2 == std::string::npos)
    
			// Start position and length
			pos1 = pos1 + HEADER.length();
			pos2 = pos2 - pos1;
			return input.substr(pos1, pos2);
		}

	}
}
