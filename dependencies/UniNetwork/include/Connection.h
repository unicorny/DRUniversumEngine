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

#ifndef __UNI_NETWORK_CONNECTION_H
#define __UNI_NETWORK_CONNECTION_H

#include "Loggable.h"
#include "Poco/Timer.h"

class UNI_NETWORK_API Connection : public Loggable
{
public:
	Connection(long periodicRunningIntervall);
	~Connection();

	// resume thread from timer
	void resume(Poco::Timer& timer);

	virtual DRReturn run() = 0;

	virtual DRNet_Status send(std::string sendRequest) = 0;
	virtual DRNet_Status recv(std::string& recvString) = 0;

protected:
	DRReturn parseJson(std::string jsonString, Json::Value& json);

	Poco::Mutex mThreadRunningMutex;
	Poco::Timer mTimer;
};

#endif __UNI_NETWORK_CONNECTION_H