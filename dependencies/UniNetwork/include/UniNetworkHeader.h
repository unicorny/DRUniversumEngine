/*/*************************************************************************
 *                                                                         *
 * UniNetwork, Library for Netwerk Communication for my					   *
 * Gameproject Spacecraft												   *			 
 * Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
 * Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.													   *
 *																		   *
 * This program is distributed in the hope that it will be useful,	       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
 * GNU General Public License for more details.							   *
 *																		   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/


#ifdef _WIN32
#define _WINSOCKAPI_
#include <windows.h>
#else
#endif

#include <UniversumLib.h> 
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include "Poco/Timer.h"
#include "Poco/Mutex.h"
#include "Poco/SynchronizedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherFactory.h"


#ifdef _WIN32
    #ifdef BUILD_DLL_UNI_NETWORK
        #define UNI_NETWORK_API __declspec(dllexport)
    #else
        #define UNI_NETWORK_API __declspec(dllimport)
    #endif
#else
    #define UNI_NETWORK_API
#endif

#include "Loggable.h"
#include "Connection.h"
#include "HTTPConnection.h"
#include "ConnectionFactory.h"
#include "UniNetwork.h"
#include "NetCrypto.h"

#ifdef __cplusplus
extern "C"
{
#endif

UNI_NETWORK_API DRINetwork* getInstance();

#ifdef __cplusplus
}
#endif

