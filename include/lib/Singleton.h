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
 * \date 11.03.15
 * 
 * \desc Base Class for singleton classes
 */
 
#ifndef __DR_UNIVERSUM_LIB_SINGLETON__
#define __DR_UNIVERSUM_LIB_SINGLETON__

#include "UniversumLib.h"

namespace UniLib {
    namespace lib {
        class UNIVERSUM_LIB_API Singleton
        {
		public:
			// Diese statische Methode erzeugt die einzige Instanz.
			// Nur über diese Methode erhalten Anwender den Zugriff auf
			// die Instanz.
			//virtual static Singleton& getInstance() = 0;
			//virtual static Singleton* getInstancePtr() = 0;
			/*{
				// Die Instanz wird erst beim ersten Aufruf erzeugt.
				if (!pInstanz)
					pInstanz = new Singleton;
				return pInstanz;
			}*/


		protected:
			// Standard- und Copykonstruktor sind private.
			// Nur Methoden dieser Klasse können auf sie zugreifen.
			Singleton() {};
			Singleton(const Singleton&) {};

			// Ein Zeiger auf die einzige Instanz
		//	static Singleton* mpInstanz;

			// Der private Destruktor verhindert, dass ein Anwender
			// delete Singleton::GetInstanz() aufrufen kann.
			virtual ~Singleton() {};

			// Es gibt nur eine Instanz. Jede Zuweisung wäre eine Selbstzuweisung.
			// Da Selbstzuweisungen selten Sinn machen, ist der op= privat
			//Singleton& operator=(const Singleton&) {return* mpInstanz;}
		};
    }
}


#endif //__DR_UNIVERSUM_LIB_SINGLETON__


