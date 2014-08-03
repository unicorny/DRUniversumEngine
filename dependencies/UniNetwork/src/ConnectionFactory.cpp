#include "UniNetworkHeader.h"

ConnectionFactory::ConnectionFactory()
{
}

ConnectionFactory::~ConnectionFactory() 
{
}

Connection* ConnectionFactory::createConnection(std::string connectionConfig)
{

	return new Connection();
}