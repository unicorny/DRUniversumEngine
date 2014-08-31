#include "UniversumLib.h"


namespace UniLib {
	namespace lib {
		Crypto::Crypto()
		{
		}
		Crypto::~Crypto()
		{
		}

		// Pure Virtual, implemantation in UniNetwork

		std::string Crypto::getOperationTypeString(OperationType type)
		{
			switch(type) {
			case CRYPT_WITH_CLIENT_PUBLIC: return std::string("crypt with client public");
			case CRYPT_WITH_CLIENT_PRIVATE: return std::string("crypt with client private");
			case UNCRYPT_WITH_SERVER_PUBLIC: return std::string("uncrypt with server public");
			case CRYPT_WITH_SERVER_PUBLIC: return std::string("crypt with server public");
			case UNCRYPT_WITH_CLIENT_PRIVATE: return std::string("uncrypt with client private");
			case UNCRYPT_WITH_CLIENT_PUBLIC: return std::string("uncrypt with client public");
			default: return std::string("");
			}
			return std::string("");
		}
	}
}
