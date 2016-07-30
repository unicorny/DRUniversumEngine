/*/*************************************************************************
 *                                                                         *
 * EngineDLL, Engine for my programs, using SDL and OpenGL		   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#ifndef __DR_UNIVERSUM_LIB_ENGINE_LOGGER__
#define __DR_UNIVERSUM_LIB_ENGINE_LOGGER__

//#include "UniversumLib.h"

/**!
 * \author: Dario Rekowski
 *
 * \date:
 * 
 * \desc: Base Thread Safe Logging class 
 *        adding stack for logging entrys to act fast in multithreaded environment
 */
namespace UniLib {
    namespace lib {
        class UNIVERSUM_LIB_API EngineLogger : public DRLogger, Thread
        {
        public:
            EngineLogger();
            virtual ~EngineLogger();

            virtual DRReturn init(const char* pcFilename, bool printToConsole);
            virtual void exit();

            //in die Log-Datei schreiben
            virtual DRReturn writeToLogDirect(DRString text);
            virtual DRReturn writeToLogDirect(const char* pcText, ...);
            virtual DRReturn writeToLog(const char* pcText, ...);
            virtual DRReturn writeToLog(DRString text);
            virtual DRReturn writeAsBinary(DRString name, u8 zahl);
            virtual DRReturn writeAsBinary(DRString name, u16 zahl);
            virtual DRReturn writeAsBinary(DRString name, u32 zahl);
            virtual DRReturn writeAsBinary(DRString name, u64 zahl);
        protected:
            
            virtual DRReturn writeAsBinary_intern(DRString name, DRString zahlBuffer);

			virtual int ThreadFunction();

			MultithreadQueue<DRString> mMessages;

        private:

        };
    }
}


#endif //__DR_UNIVERSUM_LIB_ENGINE_LOGGER__