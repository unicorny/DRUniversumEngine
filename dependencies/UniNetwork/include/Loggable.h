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

/* 
	\author: Dario Rekowski
	\date: 06.08.14
	\brief class which can log 
*/

#ifndef __UNI_NETWORK_LOGGABLE_H
#define __UNI_NETWORK_LOGGABLE_H

class UNI_NETWORK_API Loggable 
{
public: 
	Loggable(std::string logFilename, std::string className);
	~Loggable();

protected:
	std::string mLogFilename;
	std::string mClassName;
	void logFatal(std::string msg, std::string functionName);
	void logWarning(std::string msg, std::string functionName);
	void logError(std::string msg, std::string functionName);

#define POCO_LOG_WARNING(msg) logWarning(msg, __FUNCTION__)
#define POCO_LOG_ERROR(msg) logError(msg, __FUNCTION__)
#define POCO_LOG_FATAL(msg) logFatal(msg, __FUNCTION__)

private:
	void log(std::string msg, std::string functionName, Poco::Message::Priority prio);
};

#endif