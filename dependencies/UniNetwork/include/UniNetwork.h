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

#ifndef __NETWORK_MAIN_H__
#define __NETWORK_MAIN_H__

#include "Poco\Mutex.h"




// Interface klasse für Netzwerk, implementierung erfolgt in Network.dll
class UNI_NETWORK_API UniNetwork : public DRINetwork
{
public:
    virtual ~UniNetwork();

    //init and exit
    virtual DRReturn init();
    virtual void exit();


	// \brief login on server
	// \param username 
	// \param password encrypted with public key from server
	// \return true if login was successfully
	virtual bool login(std::string username, std::string password);

	// \brief connect to server
	// \param config contains server config in json format
	// \return connection number
	virtual u16 connect(std::string configJson, std::string section);

	// \brief disconnect from server
	// \param connectionNumber return value from connect
	virtual void disconnect(u16 connectionNumber);

	// \brief send data 
	virtual DRNet_Status send(std::string dataJson, u16 connectionNumber);

	// \breif recv data
	virtual DRNet_Status recv(std::string& dataJson, u16 connectionNumber);

protected:
	std::map<int, Connection*> mConnections;
	typedef std::pair<int, Connection*> CONNECTION_PAIR;

	Poco::Mutex mConnectionMutex;
	ConnectionFactory mConnectionFactory;
private:
};

#endif // __NETWORK_MAIN_H__
