/*/*************************************************************************
 *                                                                         *
 * UniversumLib, collection of classes for generating and go through a     *
 * whole universe. It is for my Gameproject Spacecraft					   * 
 * Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
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

#ifndef __UNIVERSUM_LIB_LIB_BUFFERED_NETWORK_PACKET_H__
#define __UNIVERSUM_LIB_LIB_BUFFERED_NETWORK_PACKET_H__

#include <queue>

enum DRNet_RequestTyp;

namespace UniLib {
	namespace lib {
		class UNIVERSUM_LIB_API BufferedNetworkPacket
		{
		public:
			BufferedNetworkPacket();
			~BufferedNetworkPacket();

			DRReturn pushDataWrapHTTPRequest(Json::Value json, std::string userAgent, std::string parameter, DRNet_RequestTyp requestType = NET_GET); 
			void pushData(void* data, size_t size);
			void pushData(std::string string);
			void pushData(Json::Value value);

			Json::Value popData();
			std::string popDataString();

			void setURLAndHost(std::string url, std::string host);

			
		private:
			struct DataPacket
			{
				DataPacket(void* _data, size_t _size) :data(NULL), size(_size) 
				{
					if(_size > 0) {
						data = malloc(_size);
						memcpy(data, _data, _size); 
					}
				}
				~DataPacket() 
				{
					if(data) free(data);
					data = NULL;
					size = 0;
				}
				void* data;
				size_t size;
			};
			std::queue<DataPacket*> mBufferQueue;
			SDL_mutex*				mMutex;
			bool					exit;

			std::string				mUrl;
			std::string				mHost;
		};

	};
};

#endif // __UNIVERSUM_LIB_LIB_BUFFERED_NETWORK_PACKET_H__