/*/*************************************************************************
 *                                                                         *
 * Engine, Engine-Lib interface class for many things,                     *
 * implementierung in extern libs                               	   *
 * Copyright (C) 2012, 20013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
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
//#define __NETWORK_DLL_NAME_ "Networkd.dll"
#define __NETWORK_DLL_NAME_ "UniNetworkd.dll"
#else
//#define __NETWORK_DLL_NAME_ "libNetwork.so"
#define __NETWORK_DLL_NAME_ "libUniNetwork.so"
#endif
/*
Netzwerk klasse f�r das einfache arbeiten mit sockets
Autor: Dario Rekowski
Datum: 16.09.09
*/
#include <map>

enum DRNet_Status
{
	NET_OK = 0,
    NET_NOT_READY = 1, // not ready to read or write
    NET_NOT_COMPLETE = 2, // not all writed/readed
    NET_COMPLETE = 4, // all writetd/readed
    NET_DATA_CORRUPTED = 8, // datas defekt
    NET_SOCKET_ERROR = 16, // error by creating socket
    NET_ERROR    = -1, // Fehler
	NET_NOT_FOUND = -2
};


#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
enum UNIVERSUM_LIB_API DRNet_Status;
#endif

// Interface klasse f�r Netzwerk, implementierung erfolgt in Network.dll
class UNIVERSUM_LIB_API DRINetwork : public DRInterface
{
public:
    virtual ~DRINetwork();

    // Zugrifffunktionen
    static DRINetwork* Instance();

    static inline DRINetwork& getSingleton() {return *Instance();}
    static inline DRINetwork* getSingletonPtr() {return Instance();}

    //init and exit
    virtual DRReturn init();
    virtual void exit();


	// \brief connect to server
	// \param config contains server config in json format
	// \return connection number, return 0 by error
	virtual u16 connect(std::string configJson, std::string section);

	// \brief disconnect from server
	// \param connectionNumber return value from connect
	virtual void disconnect(u16 connectionNumber);

	// \brief send data 
	virtual DRNet_Status send(std::string dataJson, u16 connectionNumber);

	// \breif recv data
	virtual DRNet_Status recv(std::string& dataJson, u16 connectionNumber);

	// helper
	virtual UniLib::lib::Crypto* createCrypto();
	virtual void freeCrypto(UniLib::lib::Crypto* crypto);

protected:
    bool mInitalized;
    static DRINetwork* mTheOneAndOnly;

    DRINetwork();

private:
};

#endif //__DR_UNIVERSUM_LIB_INETWORK__
