//#include "Engine2Main.h"
//#include "UniversumLib.h"
#include "lib/DRINetwork.h"
#include "lib/Crypto.h"


DRINetwork* DRINetwork::mTheOneAndOnly = NULL;

DRINetwork::DRINetwork()
: DRInterface(), mInitalized(false)
{
    //ctor
}

DRINetwork::~DRINetwork()
{
    close();
}

DRReturn DRINetwork::init()
{
    if(mInitalized) return DR_OK;

    LOG_INFO("INetwork init");
    mInitalized = true;

    return DR_OK;
}

void DRINetwork::exit()
{
    if(!mInitalized) return;

    LOG_INFO("INetwork exit");
    mInitalized = false;
}


DRINetwork* DRINetwork::Instance()
{
    if(!mTheOneAndOnly)
    {
        mTheOneAndOnly = (DRINetwork*)DRInterface::Instance(__NETWORK_DLL_NAME_);
        if(!mTheOneAndOnly) mTheOneAndOnly = new DRINetwork();
        return mTheOneAndOnly;
    }
    else
    {
        return mTheOneAndOnly;
    }
   // return NULL;
}

u16 DRINetwork::connect(const DRNetServerConfig& cfg)
{
    LOG_WARNING("not implemented");
    return 0;
}


void DRINetwork::disconnect(u16 connectionNumber)
{
	LOG_WARNING("not implemented");
}

DRNet_Status DRINetwork::send(const DRNetRequest& sendRequest, u16 connectionNumber)
{
    LOG_WARNING("not implemented");
    return NET_ERROR;
}

DRNet_Status DRINetwork::send(const DRNetRequest& request, u16 connectionNumber, UniLib::server::CallbackCommand* command)
{
	LOG_WARNING("not implemented");
	return NET_ERROR;
}

DRNet_Status DRINetwork::recv(std::string& dataJson, u16 connectionNumber)
{
	LOG_WARNING("not implemented");
	return NET_ERROR;
}

UniLib::lib::Crypto* DRINetwork::createCrypto() 
{
	LOG_WARNING("not implemented");
	return NULL;
}

void DRINetwork::freeCrypto(UniLib::lib::Crypto* crypto)
{
	LOG_WARNING("not implemented");
}