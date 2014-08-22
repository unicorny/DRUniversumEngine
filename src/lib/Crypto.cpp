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

		DRReturn Crypto::generateKeys()
		{

			// InvertibleRSAFunction is used directly only because the private key
			// won't actually be used to perform any cryptographic operation;
			// otherwise, an appropriate typedef'ed type from rsa.h would have been used.
			AutoSeededRandomPool rng;
			InvertibleRSAFunction privkey; 
			privkey.Initialize(rng, 1024);
			RSA::PublicKey pbkey(privkey);
			if(!pbkey.Validate(rng, 3)) {LOG_WARNING("key isn't valid");}
			else {
				UniLib::EngineLog.writeToLog("N: %d", pbkey.GetModulus());
				UniLib::EngineLog.writeToLog("E: %d", pbkey.GetPublicExponent());
			}

			// generating
			RSA::PrivateKey key1, key2;
			key1.GenerateRandomWithKeySize(rng, 3072);
	
			// saving
			// Base64 decode, place in a ByteQueue    
			ByteQueue queue, queueOut;
			Base64Encoder encoder;
			key1.Save(queue);

			encoder.Attach(new Redirector(queueOut));
			queue.CopyTo(encoder);
			encoder.MessageEnd();

			//Base64Encoder queue;
			
			std::string roundtrip;
			//FileSink file("rsa-roundtrip.key");
			StringSink file(roundtrip);
			queueOut.CopyTo(file);
			file.MessageEnd();
			// loading
			Base64Decoder loadQueue;
			//FileSource loadFile("rsa-roundtrip.key", true /*pumpAll*/);
			StringSource loadFile(roundtrip, true);

			loadFile.TransferTo(loadQueue);
			loadQueue.MessageEnd();

			key2.Load(loadQueue); 

			if(key1.GetModulus() != key2.GetModulus() ||
			   key1.GetPublicExponent() != key2.GetPublicExponent() ||
			   key1.GetPrivateExponent() != key2.GetPrivateExponent())
			{
				LOG_ERROR("key data did not round trip", DR_ERROR);
			}
			else {
				EngineLog.writeToLog("<b><font color='green'>Key Roundtrip successed</font></b>");
			}
 
			// With the current version of Crypto++, MessageEnd() needs to be called
			// explicitly because Base64Encoder doesn't flush its buffer on destruction.s

			// saving
			// Base64 decode, place in a ByteQueue    
			ByteQueue pbQueue, pbQueueOut;
			Base64Encoder pbEncoder;			

			privkey.Save(pbQueue);
			pbEncoder.Attach(new Redirector(pbQueueOut));
			pbQueue.CopyTo(pbEncoder);
			pbEncoder.MessageEnd();

			StringSink stringSink(mPrivateKey);
			pbQueueOut.CopyTo(stringSink);
			stringSink.MessageEnd();	
			
 
			// Suppose we want to store the public key separately,
			// possibly because we will be sending the public key to a third party.
			RSA::PublicKey pubKey(privkey);
			//RSAFunction pubkey(privkey);
			Base64Encoder pbEncoderNew;

			pbQueue.Clear(); pbQueueOut.Clear(); 
			pubKey.Save(pbQueue);
			pbEncoder.Attach(new Redirector(pbQueueOut));
			pbQueue.CopyTo(pbEncoder);
			pbEncoder.MessageEnd();

			StringSink stringSink2(mPublicKey);
			pbQueueOut.CopyTo(stringSink2);
			stringSink2.MessageEnd();

			//mPublicKey = roundtrip;

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

		DRReturn Crypto::setServerPublicKey(std::string pbKey, int validationLevel)
		{
			return getPublicKeyFromPEM(pbKey,  mServerPublic);
			generateKeys();
			try {
				//StringSource string(pbKey, true);
				StringSource string(pbKey, true, new Base64Decoder);
				ByteQueue queue;
				string.TransferTo(queue);
				queue.MessageEnd();
				//RSA::PublicKey p;
				RSA::PrivateKey p;
				//p.Load(queue);
				//p.BERDecodePrivateKey(queue,false,0); // last 2 params unused
				p.BERDecodePrivateKey(queue, false /*paramsPresent*/, queue.MaxRetrievable());
			} 
			catch(CryptoPP::BERDecodeErr err) 
			{
				UniLib::EngineLog.writeToLog("CryptoLib Exception: %s", err.what());
				LOG_ERROR("Excpetion occured during loading key", DR_ERROR);
			}
			//mServerPublic.Load(queue);
			AutoSeededRandomPool rng;
			if(!mServerPublic.Validate(rng, validationLevel)) 
				LOG_ERROR("Error, key isn't good enough for our purpose", DR_ERROR);
			return DR_OK;
		}

	
		DRReturn Crypto::getPublicKeyFromPEM(std::string pemPublickKey, CryptoPP::RSA::PublicKey& pbKey)
		{
			static std::string HEADER = "-----BEGIN PUBLIC KEY-----\n";
			static std::string FOOTER = "\n-----END";
    
			size_t pos1, pos2;
			pos1 = pemPublickKey.find(HEADER);
			if(pos1 == std::string::npos)
				LOG_ERROR("PEM header not found", DR_ERROR);
    
			pos2 = pemPublickKey.find(FOOTER, pos1+1);
			if(pos2 == std::string::npos)
				LOG_ERROR("PEM footer not found, DR_ERROR", DR_ERROR);
    
			// Start position and length
			pos1 = pos1 + HEADER.length();
			pos2 = pos2 - pos1;
			std::string keystr = pemPublickKey.substr(pos1, pos2);
			//std::string keystr = mPublicKey;
			//StringSource ss(mPublicKey, true);
			StringSource ss(keystr, true);
			//*/
			//std::string keystr = pemPublickKey;

			// Base64 decode, place in a ByteQueue    
			ByteQueue queue2, queue;
			Base64Decoder decoder;
			ss.TransferTo(decoder);
			decoder.MessageEnd();
			/*

			decoder.Attach(new Redirector(queue));
			//decoder.Put((const byte*)keystr.data(), keystr.length());
			ss.CopyTo(decoder);
			decoder.MessageEnd();			

			// Write to file for inspection
			FileSink fs("decoded-key.der");
			queue.CopyTo(fs);
			fs.MessageEnd();

			/*FileSource fin("rsa-openssl.der", true);
			fin.TransferTo(queue);
			queue.MessageEnd();
			*/

			try
			{
				//CryptoPP::RSA::PrivateKey rsaPrivate;
				//rsaPrivate.BERDecodePrivateKey(queue, false /*paramsPresent*/, queue.MaxRetrievable());
				//pbKey.BERDecodePublicKey(queue, false /*paramsPresent*/, queue.MaxRetrievable());
				pbKey.Load(decoder);
			//	RSA::PrivateKey prKey;
				//prKey.Load(decoder);

				// BERDecodePrivateKey is a void function. Here's the only check
				// we have regarding the DER bytes consumed.
				//ASSERT(queue.IsEmpty());
				if(!queue.IsEmpty()) LOG_ERROR("some bytes not decoded", DR_ERROR);
    
				AutoSeededRandomPool prng;
				//bool valid = rsaPrivate.Validate(prng, 3);
				//bool valid = pbKey.Validate(prng, 3);
				bool valid = pbKey.Validate(prng, 3);
				if(!valid) {
					LOG_ERROR("RSA public key is not valid", DR_ERROR);
				}
				UniLib::EngineLog.writeToLog("N: %d", pbKey.GetModulus());
				UniLib::EngineLog.writeToLog("E: %d", pbKey.GetPublicExponent());
				//cout << "N:" << rsaPrivate.GetModulus() << endl;
				//cout << "E:" << rsaPrivate.GetPublicExponent() << endl;
				//cout << "D:" << rsaPrivate.GetPrivateExponent() << endl;
    
			}
			catch (const Exception& ex)
			{
				UniLib::EngineLog.writeToLog(ex.what());
				LOG_ERROR("error by decoding", DR_ERROR);
			}
			return DR_OK;
		}
	}
}

/*
void Load(const string& filename, BufferedTransformation& bt)
{
    FileSource file(filename.c_str(), true);

    file.TransferTo(bt);
    bt.MessageEnd();
}

void LoadPublicKey(const string& filename, PublicKey& key)
{
    ByteQueue queue;
    Load(filename, queue);

    key.Load(queue);    
}
if(!rsaPrivate.Validate(rnd, 3))
    throw runtime_error("Rsa private key validation failed");
	//*/