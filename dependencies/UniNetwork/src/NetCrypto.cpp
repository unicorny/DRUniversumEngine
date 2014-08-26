#include "UniNetworkHeader.h"


NetCrypto::NetCrypto()
{
}

NetCrypto::~NetCrypto()
{
}

std::string NetCrypto::crypt(std::string input, OperationType type)
{
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
	return DR_OK;
}

DRReturn NetCrypto::setServerPublicKey(std::string e, std::string n, int validationLevel/* = 3*/)
{
	return DR_OK;
}

std::string NetCrypto::getServerPublicKey()
{
	return "";
}