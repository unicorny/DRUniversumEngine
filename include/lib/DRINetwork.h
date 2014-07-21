/*/*************************************************************************
 *                                                                         *
 * Engine, Engine-Lib interface class for many things,                     *
 * implementierung in extern libs                               	   *
 * Copyright (C) 2012, 20013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRAN TY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#ifndef __DR_UNIVERSUM_LIB_INETWORK__
#define __DR_UNIVERSUM_LIB_INETWORK__
#ifdef _WIN32
#define __NETWORK_DLL_NAME_ "Networkd.dll"
#else
#define __NETWORK_DLL_NAME_ "libNetwork.so"
#endif
/*
Netzwerk klasse f�r das einfache arbeiten mit sockets
Autor: Dario Rekowski
Datum: 16.09.09
*/
#include <map>

enum DRNet_Status
{
    NET_NOT_READY = 1, // not ready to read or write
    NET_NOT_COMPLETE = 2, // not all writed/readed
    NET_COMPLETE = 4, // all writetd/readed
    NET_DATA_CORRUPTED = 8, // datas defekt
    NET_SOCKET_ERROR = 16, // error by creating socket
    NET_ERROR    = -1 // Fehler
};

#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
enum UNIVERSUM_LIB_API DRNet_Status;
#endif

enum DRNet_RequestTyp
{
    NET_POST = 1,
    NET_GET = 2
};

#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
enum UNIVERSUM_LIB_API DRNet_RequestTyp;
#endif

namespace UniLib {
	namespace lib {
		class BufferedNetworkPacket;
	}
}

// Interface klasse f�r Netzwerk, implementierung erfolgt in Network.dll
class UNIVERSUM_LIB_API DRINetwork : public DRInterface
{
public:
    virtual ~DRINetwork();

    // Zugrifffunktionen
    static DRINetwork* Instance();

    static inline DRINetwork& getSingleton() {return *Instance();}
    static inline DRINetwork* getSingletonPtr() {return Instance();}

    //init und exit
    virtual DRReturn init();
    virtual void exit();

    // zugriff/arbeits funktionen
    //! \brief baut eine verbindung auf
    //! \param host_ip histname oder ip adresse (durch . getrennnte ziffern)
    //! \param port port number, 80 f�r http
    //! \return verbindungs ID oder null bei Fehler
    virtual int connect(const char* host_ip, int port);

	virtual DRReturn createBufferedConnection(const char* name, const char* host_ip, int port,
										UniLib::lib::BufferedNetworkPacket* inputBuffer, UniLib::lib::BufferedNetworkPacket* outputBuffer);

	virtual int removeBufferedConnection(const char* name);
    //! \brief to make a HTTP Request at the target host
    //! \param url complete url
    //! \param request typ, POST oder GET
    //! \param parameter, string containing parameter, will be added at request
    //! \return dataIndex zum empfangen der antwort oder 0 bei Fehler
    virtual int HTTPRequest(const char* url, DRNet_RequestTyp typ, const std::string& parameter, const char* userAgent = "Freies Leben");

    //! \brief zum erhalen der antworten/ read answears
    //! \param dataIndex die nummer des datenaustausches
    //! \param buffer buffer des aufrufers zum erhalten der daten
    //! \param bufferSize die Gr��e des Buffers des Aufrufers
    virtual DRNet_Status getData(int dataIndex, void* buffer, int bufferSize);


	virtual DRReturn update(float timeSinceLastFrame);

protected:
    bool mInitalized;
    static DRINetwork* mTheOneAndOnly;

    DRINetwork();

private:
};

#endif //__DR_UNIVERSUM_LIB_INETWORK__
