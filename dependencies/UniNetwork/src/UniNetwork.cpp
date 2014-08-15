#include "UniNetworkHeader.h"


using namespace std;

UniNetwork::~UniNetwork()
{
}

    //init and exit
DRReturn UniNetwork::init()
{
	Poco::Net::initializeNetwork();
	return DR_OK;
}

void UniNetwork::exit()
{
	Poco::Net::uninitializeNetwork();
}

bool UniNetwork::login(std::string username, std::string password)
{
	return false;
}

// \brief connect to server
// \param config contains server config in json format
// \return connection number
u16 UniNetwork::connect(std::string configJson, std::string section)
{
	Connection* con = mConnectionFactory.createConnection(configJson, section);
	//if(!con) LOG_ERROR("Error by creating connection", 0);
	
	mConnectionMutex.lock();
	u16 key = mConnections.size()+1;
	mConnections.insert(CONNECTION_PAIR(key, con));
	mConnectionMutex.unlock();
	return key;
}

// \brief disconnect from server
// \param connectionNumber return value from connect
void UniNetwork::disconnect(u16 connectionNumber)
{
	mConnectionMutex.lock();
	DR_SAVE_DELETE(mConnections[connectionNumber]);
	mConnections.erase(connectionNumber);
	mConnectionMutex.unlock();
}

// \brief send data 
DRNet_Status UniNetwork::send(std::string dataJson, u16 connectionNumber)
{
	if(!mConnections[connectionNumber]) return NET_NOT_FOUND;
	return mConnections[connectionNumber]->send(dataJson);
}

// \breif recv data
DRNet_Status UniNetwork::recv(std::string& dataJson, u16 connectionNumber)
{
	return NET_ERROR;
}