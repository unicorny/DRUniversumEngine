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

/*!
 * \author Dario Rekowski
 * 
 * \date 25.10.2015
 * 
 * \desc Input Camera Class for Game Cameras controlled by player/Controls
 */

#ifndef __UNIVESUM_LIB_CONTROLLE_INPUT_CAMERA_H
#define __UNIVESUM_LIB_CONTROLLE_INPUT_CAMERA_H

#include "Camera.h"
#include "InputCommand.h"

namespace UniLib {
	namespace controller {
		class UNIVERSUM_LIB_API InputCamera : public Camera, public InputCommand
		{
		public: 
			InputCamera(float mvSpeed, float rotateSpeed);
			virtual ~InputCamera();

			virtual DRReturn input(InputCommandEnum in);
		protected:
			float mMoveSpeed;
			float mRotationSpeed;
		};
	}
}

#endif //__UNIVESUM_LIB_CONTROLLE_INPUT_CAMERA_H