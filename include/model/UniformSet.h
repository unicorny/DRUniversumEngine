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


#ifndef __DR_UNIVERSUM_LIB_MODEL_UNIFORM_SET__H
#define __DR_UNIVERSUM_LIB_MODEL_UNIFORM_SET__H

#include "UniversumLib.h"

namespace UniLib {
    namespace model {
        
        /*!
         * \author Dario Rekowski
         * 
         * \date 13.04.15
         * 
         * \brief save uniform data set 
         * build like drink amount data from trink lotse
         * use a dynamic array for uniform data (cache hit)
         * simply said it is only a container for storing different data types in one list
         * can be easy implemented with templates
         */
        class UNIVERSUM_LIB_API UniformSet
        {
        public:
            UniformSet();
            ~UniformSet();

            DRReturn setUniform(std::string name, int value);
            DRReturn setUniform(std::string name, float value);
            DRReturn setUniform(std::string name, DRVector2 value);
            DRReturn setUniform(std::string name, DRVector3 value);
            DRReturn setUniform(std::string name, DRColor value);
            DRReturn setUniform(std::string name, DRVector3i value);
            DRReturn setUniform(std::string name, DRVector2i value);

            
        protected:
            enum UniformEntryTypes {
                UNIFORM_INT = 0,
                UNIFORM_FLOAT = 1,
                UNIFORM_VEC2 = 2,
                UNIFORM_VEC2I = 3,
                UNIFORM_VEC3 = 4,
                UNIFORM_VEC3I = 5,
                UNIFORM_VEC4 = 6
            };
            struct UniformEntry
            {
                UniformEntry(): type(UNIFORM_INT), data(0), dirtyFlag(false) {}
                UniformEntry(UniformEntryTypes type, int data, std::string name)
                   : type(type), data(data), name(name), dirtyFlag(true) {}
                ~UniformEntry();
                // ---------------------------------------------
                UniformEntryTypes type;
                int data;
                std::string name;
                bool dirtyFlag;
            };
            std::map<int, UniformEntry*> mUniformEntrys;

            DRReturn addUniform(UniformEntry* newUniform);
        };

        // *****************************************************************
        // store all uniform in memory directly next, if it make sense 
        class UNIVERSUM_LIB_API FastUniformSet: public UniformSet
        {

        };
    };
};
#endif //__DR_UNIVERSUM_LIB_MODEL_UNIFORM_SET__H