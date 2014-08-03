#include "UniNetworkHeader.h"


using namespace std;

UniNetwork::~UniNetwork()
{
}

    //init and exit
DRReturn UniNetwork::init()
{
	return DR_OK;
}

void UniNetwork::exit()
{
}

// \brief connect to server
// \param config contains server config in json format
// \return connection number
u16 UniNetwork::connect(std::string configJson)
{
	mConnectionMutex.lock();
	mConnectionMutex.unlock();
	return 0;
}

// \brief disconnect from server
// \param connectionNumber return value from connect
void UniNetwork::disconnect(u16 connectionNumber)
{
	mConnectionMutex.lock();
	mConnectionMutex.unlock();
}

// \brief send data 
DRNet_Status UniNetwork::send(std::string dataJson, u16 connectionNUmber)
{
	return NET_ERROR;
}

// \breif recv data
DRNet_Status UniNetwork::recv(std::string& dataJson, u16 connectionNumber)
{
	return NET_ERROR;
}