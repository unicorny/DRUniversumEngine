/*/*************************************************************************
*                                                                         *
* UniversumLib, collection of classes for generating and go through a     *
* whole universe. It is for my Gameproject Spacecraft					   *
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

/**
 * \author Dario Rekowski copied from: http://fara.cs.uni-potsdam.de/~kaufmann/?page=GenCppFaqs&faq=Singleton
 * 
 * \date 16.03.15
 * 
 * \desc Strcutures for Network and other libs
 */
 
#ifndef __DR_UNIVERSUM_LIB_STRUCTURES__
#define __DR_UNIVERSUM_LIB_STRUCTURES__

#include "UniversumLib.h"

enum DRNet_Request_Method {
	NET_REQUEST_METHOD_NONE = 0,
	NET_REQUEST_METHOD_GET = 1,
	NET_REQUEST_METHOD_POST = 2
};

enum DRNet_Request_ContentType
{
	//application/x-www-form-urlencoded
	NET_REQUEST_CONTENT_TYPE_NONE = 0,
	NET_REQUEST_CONTENT_TYPE_FORM_URLENCODED = 1
};
/*
	contains infos for a simple request
*/
struct UNIVERSUM_LIB_API DRNetRequest
{
	std::string url;
	DRNet_Request_Method method;
	DRNet_Request_ContentType contentType;
	std::string userAgent;
	Json::Value content; // json encoded content

	// methods
	DRNetRequest();

	static DRNet_Request_Method getRequestMethodFromString(std::string requestMethodString);
	static const char* getRequestMethodAsString(DRNet_Request_Method requestMethod);

	static DRNet_Request_ContentType getRequestContentTypeFromString(std::string contentTypeString);
	static const char* getRequestContentTypeAsString(DRNet_Request_ContentType contentType);
};

enum DRNet_Server_Type
{
	NET_SERVER_TYPE_NONE = 0,
	NET_SERVER_TYPE_HTTP = 1,
	NET_SERVER_TYPE_SOCKET = 2
};
/*
	contain every data needed to create a server connection
*/
struct UNIVERSUM_LIB_API DRNetServerConfig
{
	std::string url;//or ip
	int port;
	std::string route;
	DRNet_Server_Type serverType;

	// methods
	void readFromJson(Json::Value value);
	static const char* getServerTypeString(DRNet_Server_Type type);
	static DRNet_Server_Type getServerTypeFromString(std::string typseString);
};

namespace UniLib {
	namespace lib {


	}
}

#endif //__DR_UNIVERSUM_LIB_STRUCTURES__