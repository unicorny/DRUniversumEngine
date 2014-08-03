#include "Connection.h"
//#include "DRINetwork.h"

using namespace std;

Connection::Connection(const char* url_host, int port)
: mURL(""), mHOST(""), mSocket(NULL), mCallbackIndex(0)
{
    //ctor
    mIP.port = port;
    splitURL_HOST(url_host);
	mSocketSet = SDLNet_AllocSocketSet(1);
}

Connection::~Connection()
{
    //dtor
    if(mSocket)
	{
		SDLNet_TCP_DelSocket(mSocketSet, mSocket);
        SDLNet_TCP_Close(mSocket);
	}
	SDLNet_FreeSocketSet(mSocketSet);
}

TCPsocket Connection::getSocket()
{
    if(!mSocket)
    {
        if(getIP()) LOG_ERROR("ung�ltige IP", NULL);
        mSocket = SDLNet_TCP_Open(&mIP);
		SDLNet_TCP_AddSocket(mSocketSet, mSocket);
        if(!mSocket) LOG_ERROR_SDL(NULL);
    }

    return mSocket;
}

DRReturn Connection::getIP()
{
    DRLog.writeToLog("host: %d, port: %d, host: %s", mIP.host, mIP.port, mHOST.data());
    int ret = SDLNet_ResolveHost(&mIP, mHOST.data(), mIP.port);
    DRLog.writeToLog("resolve Host: %d, host: %d, port: %d", ret, mIP.host, mIP.port);
    if(ret == 0) return DR_OK;

    //host could't be resolve, we look if we have get a ip adresse

    unsigned char count = 0;
    unsigned int host = 0;
    std::string tempStr;
    for(unsigned int i = 0; i < mHOST.length(); i++)
    {
        char c = mHOST[i];
        if(c != '.')
        {
            tempStr += c;
        }
        else
        {
            //mIP.host
            int temp = atoi(tempStr.data());
            host |= (temp << count*4);
            count++;
            if(count > 3) LOG_ERROR("ip string ist ung�ltig", DR_ERROR);
        }
    }
    int temp = atoi(tempStr.data());
    host |= temp << 3*4;
    SDLNet_Write32(host, &mIP.host);

    return DR_OK;
}

void Connection::splitURL_HOST(const char* url_host)
{
    string temp = url_host;
    string::size_type first = temp.find("\\");
    if(first == temp.npos)
    {
        first = temp.find("/");
        if(first == temp.npos)
        {
            mHOST = temp;
            mURL = "";
            return;
        }
    }
    mHOST = temp.substr(0, first);
    mURL = temp.substr(first, temp.npos);

    DRLog.writeToLog("Host: %s, URL: %s", mHOST.data(), mURL.data());
}

UniLib::lib::BufferedNetworkPacket* Connection::send(const void* data, int length)
{
    LOG_INFO("send");
	TCPsocket socket = getSocket();
    if(!socket) return NULL;

    int sended = SDLNet_TCP_Send(socket, data, length);
    DRLog.writeToLog("%d bytes gesendet", sended);

    /* temp */
    char buffer[2084];
    memset(buffer, 0, 2084);
    int ret = 0;
    int readedBytes = 0;
	UniLib::lib::BufferedNetworkPacket* packetBuffer = new UniLib::lib::BufferedNetworkPacket;
    while(ret = SDLNet_TCP_Recv(socket, &buffer[readedBytes], 2048-readedBytes))
    {
        readedBytes += ret;
        if(readedBytes >= 2048)
        {
            LOG_WARNING("buffer ist zu klein f�r empfangene Daten");
            //memset(buffer, 0, 2084);
            DRLog.writeToLog("ret: %d, readedBytes: %d", ret, readedBytes);
			packetBuffer->pushData(buffer, readedBytes);
    //        fwrite(buffer, 1, 2048, out);
            readedBytes = ret = 0;
            DRLog.writeToLogDirect("\n\nbuffer: %s", buffer);

            //break;
        }
    }
    
    //LOG_WARNING_SDL();
   DRLog.writeToLog("buffer: <pre>%s</pre>", buffer);
    /* */

	if(sended == length) return packetBuffer;

    LOG_ERROR("nicht alles gesendet", NULL);
}
